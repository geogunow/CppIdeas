/**
 * @file TraverseSegments.h
 * @brief A TraverseSegments object
 * @date February 15, 2016
 * @author Geoffrey Gunow, MIT, Course 22 (geogunow@mit.edu)
 */

#ifndef TRAVERSE_SEGMENTS_H_
#define TRAVERSE_SEGMENTS_H_

#ifdef SWIG
#include "Python.h"
#endif
#include "Track2D.h"
#include "Track3D.h"
#include "Geometry.h"


template <typename T>
class TraverseSegments<T> {

protected:

  /** Pointer to the associate TrackGenerator */
  TrackGenerator* _track_generator;

public:

  TraverseSegments(segmentationType type);
  virtual ~TraverseSegments();

  /* Set parameters */

  /* Get parameters */

  /* Functions to loop over tracks and do work */
  void execute();
  void pre();
  void loopOverTracks();
  void onTrack();
  void post();

};
