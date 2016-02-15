#include "TraverseSegments.h"

/**
 * @brief Constructor for the TravseSegments class assigns the TrackGenerator
 */
template <typename T>
TraverseSegments::TraverseSegments<T>(TrackGenerator* track_generator) {
  _track_generator = track_generator;
  //TODO: extract information from track_generator
}


void TraverseSegments::execute<T>() {
  pre(); //make kernels, etc
  loopOverTracks();
  post(); //delete kernels, etc
}

void TraverseSegments::pre<T>() {}

void TraverseSegments::post<T>() {}

void TraverseSegments::loopOverTracks<OTF_TRACKS>() {

#pragma omp for
  /* Loop over flattened 2D tracks */
  for (int ext_id=0; ext_id < _num_2D_tracks; ext_id++) {
    
    /* Extract indices of 3D tracks associated with the flattened track */
    Track* flattened_track = _flattened_tracks[ext_id];
    int a = flattened_track->getAzimIndex();
    int azim_index = _quadrature->getFirstOctantAzim(a);
    int i = flattened_track->getXYIndex();

    /* Loop over polar angles */
    for (int p=0; p < _num_polar; p++) {

      /* Loop over tracks in the z-stack */
      for (int z=0; z < _tracks_per_stack[a][i][p]; z++) {

        /* Extract 3D track and initialize segments pointer */
        Track* track_3D = &_tracks_3D[a][i][p][z];

        /* Trace the segments on the track */
        traceSegmentsOTF(flattened_track, start, theta, _kernels[0]);

        /* Apply kernel to track */
        onTrack(track_3D, _kernels[0]); // This will be application specific
      }
    }
  }
}

void TraverseSegments::loopOverTracks<OTF_STACKS>() {

#pragma omp for
  /* Loop over flattened 2D tracks */
  for (int ext_id=0; ext_id < _num_2D_tracks; ext_id++) {
    
    /* Extract indices of 3D tracks associated with the flattened track */
    Track* flattened_track = _flattened_tracks[ext_id];
    int a = flattened_track->getAzimIndex();
    int azim_index = _quadrature->getFirstOctantAzim(a);
    int i = flattened_track->getXYIndex();

    /* Loop over polar angles */
    for (int p=0; p < _num_polar; p++) {
      
      /* Trace all tracks in the z-stack */      
      for (int z = 0; z < _max_num_tracks_per_stack; z++)
        kernels[z]->resetCount();
      traceStackOTF(flattened_track, p, kernels);

      /* Loop over tracks in the z-stack */
      for (int z=0; z < _tracks_per_stack[a][i][p]; z++) {

        /* Extract 3D track and initialize segments pointer */
        Track* track_3D = &_tracks_3D[a][i][p][z];

        /* Apply kernel to track */
        onTrack(track_3D, _kernels[z]); // This will be application specific
      }
    }
  }
}


