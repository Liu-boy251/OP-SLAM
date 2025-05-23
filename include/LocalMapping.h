#ifndef LOCALMAPPING_H
#define LOCALMAPPING_H

#include "KeyFrame.h"
#include "Map.h"
#include "LoopClosing.h"
#include "Tracking.h"
#include "KeyFrameDatabase.h"

#include <mutex>


namespace Planar_SLAM
{

    class Tracking;
    class LoopClosing;
    class Map;

    class LocalMapping
    {
    public:

        LocalMapping(Map* pMap, const float bMonocular);

        void SetLoopCloser(LoopClosing* pLoopCloser);

        void SetTracker(Tracking* pTracker);

        // Main function
        void Run();

        void InsertKeyFrame(KeyFrame* pKF);

        // Thread Synch
        void RequestStop();
        void RequestReset();
        bool Stop();
        void Release();
        bool isStopped();
        bool stopRequested();
        bool AcceptKeyFrames();
        void SetAcceptKeyFrames(bool flag);
        bool SetNotStop(bool flag);

        void InterruptBA();

        void RequestFinish();
        bool isFinished();

        int KeyframesInQueue(){
            unique_lock<std::mutex> lock(mMutexNewKFs);
            return mlNewKeyFrames.size();
        }

    protected:

        bool CheckNewKeyFrames();
        void ProcessNewKeyFrame();
        void CreateNewMapPoints();
        void CreateNewMapLines1();
        void CreateNewMapLines2();

        void MapPointCulling();
        void SearchInNeighbors();

        void MapLineCulling();
        void MapPlaneCulling();

        void KeyFrameCulling();

        cv::Mat ComputeF12(KeyFrame* &pKF1, KeyFrame* &pKF2);

        cv::Mat SkewSymmetricMatrix(const cv::Mat &v);

        bool mbMonocular;

        void ResetIfRequested();
        bool mbResetRequested;
        std::mutex mMutexReset;

        bool CheckFinish();
        void SetFinish();
        bool mbFinishRequested;
        bool mbFinished;
        std::mutex mMutexFinish;

        Map* mpMap;

        LoopClosing* mpLoopCloser;
        Tracking* mpTracker;

        std::list<KeyFrame*> mlNewKeyFrames;

        KeyFrame* mpCurrentKeyFrame;

        std::list<MapPoint*> mlpRecentAddedMapPoints;

        std::list<MapLine*> mlpRecentAddedMapLines;

        std::list<MapPlane*> mlpRecentAddedMapPlanes;

        std::mutex mMutexNewKFs;

        bool mbAbortBA;

        bool mbStopped;
        bool mbStopRequested;
        bool mbNotStop;
        std::mutex mMutexStop;

        bool mbAcceptKeyFrames;
        std::mutex mMutexAccept;
    };

} //namespace OP_SLAM

#endif // LOCALMAPPING_H
