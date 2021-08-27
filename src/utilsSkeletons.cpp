//
// Created by andrei on 25.01.21.
//

#pragma clang diagnostic push
#pragma ide diagnostic ignored "cert-err58-cpp"

#include <utils/utilsSkeletons.h>

using namespace op;
using namespace std;

const map<PoseModel, SkeletonDetectionModel> &OPENPOSE_TO_DETECTION_MODEL = {
        {PoseModel::BODY_25, SkeletonDetectionModel::OPENPOSE},
        {PoseModel::COCO_18, SkeletonDetectionModel::OPENPOSE_18},
        {PoseModel::MPI_15,  SkeletonDetectionModel::OPENPOSE_15},
};

const map<SkeletonDetectionModel, int> &MODEL_JOINTS = {
        {SkeletonDetectionModel::OPENPOSE,    25},
        {SkeletonDetectionModel::OPENPOSE_18, 18},
        {SkeletonDetectionModel::OPENPOSE_15, 15},
        {SkeletonDetectionModel::KINECT,      20},
};

const map<SkeletonDetectionModel, map<unsigned int, string>> &ALL_POSE_BODY_PART_MAPPINGS = {
        {SkeletonDetectionModel::OPENPOSE,    getPoseBodyPartMapping(PoseModel::BODY_25)},
        {SkeletonDetectionModel::OPENPOSE_18, getPoseBodyPartMapping(PoseModel::COCO_18)},
        {SkeletonDetectionModel::OPENPOSE_15, getPoseBodyPartMapping(PoseModel::MPI_15)},
        {SkeletonDetectionModel::KINECT,      {
                                                      {0, "Nose"},  // Nose
                                                      {1, "Neck"},  // Neck
                                                      {2, "Low Spine"},  // Low Spine
                                                      {3, "Mid Hip"},  // Mid Hip
                                                      {4, "Right Shoulder"},  // Right Shoulder
                                                      {5, "Right Elbow"},  // Right Elbow
                                                      {6, "Right Wrist"},  // Right Wrist
                                                      {7, "Right Hand Tip"},  // Right Hand Tip
                                                      {8, "Left Shoulder"},  // Left Shoulder
                                                      {9, "Left Elbow"},  // Left Elbow
                                                      {10, "Left Wrist"},  // Left Wrist
                                                      {11, "Left Hand Tip"},  // Left Hand Tip
                                                      {12, "Right Hip"},  // Right Hip
                                                      {13, "Right Knee"},  // Right Knee
                                                      {14, "Right Ankle"},  // Right Ankle
                                                      {15, "Right Foot Tip"},  // Right Foot Tip
                                                      {16, "Left Hip"},  // Left Hip
                                                      {17, "Left Knee"},  // Left Knee
                                                      {18, "Left Ankle"},  // Left Ankle
                                                      {19, "Left Foot Tip"},  // Left Foot Tip
                                              }},
};

const map<SkeletonDetectionModel, vector<unsigned int>> &ALL_POSE_PART_PAIRS = {
        {SkeletonDetectionModel::OPENPOSE,    getPosePartPairs(PoseModel::BODY_25)},
        {SkeletonDetectionModel::OPENPOSE_18, getPosePartPairs(PoseModel::COCO_18)},
        {SkeletonDetectionModel::OPENPOSE_15, getPosePartPairs(PoseModel::MPI_15)},
        {SkeletonDetectionModel::KINECT,      {
                                                      0, 1,
                                                      1, 2,
                                                      2, 3,
                                                      1, 4,
                                                      4, 5,
                                                      5, 6,
                                                      6, 7,
                                                      1, 8,
                                                      8, 9,
                                                      9, 10,
                                                      10, 11,
                                                      3, 12,
                                                      12, 13,
                                                      13, 14,
                                                      14, 15,
                                                      3, 16,
                                                      16, 17,
                                                      17, 18,
                                                      18, 19
                                              }},
};

const map<SkeletonDetectionModel, vector<unsigned int>> &ALL_POSE_MAP_INDEX = {
        {SkeletonDetectionModel::OPENPOSE,    getPoseMapIndex(PoseModel::BODY_25)},
        {SkeletonDetectionModel::OPENPOSE_18, getPoseMapIndex(PoseModel::COCO_18)},
        {SkeletonDetectionModel::OPENPOSE_15, getPoseMapIndex(PoseModel::MPI_15)},
        {SkeletonDetectionModel::KINECT,      {}},
};

const map<SkeletonDetectionModel, vector<double>> &ALL_POSE_COLORS = {
        {SkeletonDetectionModel::OPENPOSE,    vector<double>{POSE_BODY_25_COLORS_RENDER_GPU}},
        {SkeletonDetectionModel::OPENPOSE_18, vector<double>{POSE_COCO_COLORS_RENDER_GPU}},
        {SkeletonDetectionModel::OPENPOSE_15, vector<double>{POSE_MPI_COLORS_RENDER_GPU}},
        {SkeletonDetectionModel::KINECT,      {
                                                      255, 0, 0,  // Nose
                                                      255, 0, 85,  // Neck
                                                      255, 0, 170, // LowSpine
                                                      255, 0, 255, // MidHip

                                                      255, 63, 0,  // RShoulder
                                                      255, 126, 0, // RElbow
                                                      255, 189, 0, // RWrist
                                                      255, 255, 0, // RHand

                                                      189, 255, 0, // LShoulder
                                                      126, 255, 0, // LElbow
                                                      63, 255, 0, // LWrist
                                                      0, 255, 0, // LHand

                                                      0, 255, 63, // RHip
                                                      0, 255, 126, // RKnee
                                                      0, 255, 189, // RAnkle
                                                      0, 255, 255, // RFoot

                                                      0, 189, 255, // LHip
                                                      0, 126, 255, // LKnee
                                                      0, 63, 255, // LAnkle
                                                      0, 0, 255, // LFoot
                                              }},
};

const map<SkeletonBodyPart, string> &SKELETON_BODY_PART_NAMES = {
        {NECK,      "neck"},
        {HIP,       "mid hip"},
        {HEAD,      "head"},
        {LEFT_ARM,  "left arm"},
        {RIGHT_ARM, "right arm"},
        {LEFT_LEG,  "left leg"},
        {RIGHT_LEG, "right leg"}
};

const map<SkeletonDetectionModel, map<SkeletonBodyPart, vector<int>>> &BODY_PART_TO_JOINTS = {
        {SkeletonDetectionModel::OPENPOSE,    {
                                                      {HEAD, {0, 15, 16, 17, 18}},
                                                      {NECK, {1}},
                                                      {HIP, {8}},
                                                      {RIGHT_ARM, {2, 3, 4}},
                                                      {LEFT_ARM,  {5, 6, 7}},
                                                      {RIGHT_LEG, {9,  10, 11, 22, 23, 24}},
                                                      {LEFT_LEG,  {12, 13, 14, 19, 20, 21}},
                                              }},
        {SkeletonDetectionModel::OPENPOSE_18, {
                                                      {HEAD, {}},
                                                      {NECK, {}},
                                                      {HIP, {}},
                                                      {LEFT_ARM,  {}},
                                                      {RIGHT_ARM, {}},
                                                      {LEFT_LEG,  {}},
                                                      {RIGHT_LEG, {}},
                                              }},
        {SkeletonDetectionModel::OPENPOSE_15, {
                                                      {HEAD, {}},
                                                      {NECK, {}},
                                                      {HIP, {}},
                                                      {LEFT_ARM,  {}},
                                                      {RIGHT_ARM, {}},
                                                      {LEFT_LEG,  {}},
                                                      {RIGHT_LEG, {}},
                                              }},
        {SkeletonDetectionModel::KINECT,      {
                                                      {HEAD, {0}},
                                                      {NECK, {1}},
                                                      {HIP, {2, 3}},
                                                      {RIGHT_ARM, {4, 5, 6, 7}},
                                                      {LEFT_ARM,  {8, 9, 10, 11}},
                                                      {RIGHT_LEG, {12, 13, 14, 15}},
                                                      {LEFT_LEG,  {16, 17, 18, 19}},
                                              }},
};

const map<SkeletonDetectionModel, map<SkeletonBodyPart, int>> &MAX_ATTENTION_LEVELS = {
        {SkeletonDetectionModel::OPENPOSE,    {
                                                      {HEAD, 2},
                                                      {NECK, 0},
                                                      {HIP, 0},
                                                      {LEFT_ARM, 2},
                                                      {RIGHT_ARM, 2},
                                                      {LEFT_LEG, 3},
                                                      {RIGHT_LEG, 3},
                                              }},
        {SkeletonDetectionModel::OPENPOSE_18, {
                                                      {HEAD, 0},
                                                      {NECK, 0},
                                                      {HIP, 0},
                                                      {LEFT_ARM, 0},
                                                      {RIGHT_ARM, 0},
                                                      {LEFT_LEG, 0},
                                                      {RIGHT_LEG, 0},
                                              }},
        {SkeletonDetectionModel::OPENPOSE_15, {
                                                      {HEAD, 0},
                                                      {NECK, 0},
                                                      {HIP, 0},
                                                      {LEFT_ARM, 0},
                                                      {RIGHT_ARM, 0},
                                                      {LEFT_LEG, 0},
                                                      {RIGHT_LEG, 0},
                                              }},
        {SkeletonDetectionModel::KINECT,      {
                                                      {HEAD, 0},
                                                      {NECK, 0},
                                                      {HIP, 1},
                                                      {LEFT_ARM, 3},
                                                      {RIGHT_ARM, 3},
                                                      {LEFT_LEG, 3},
                                                      {RIGHT_LEG, 3},
                                              }},
};

const map<SkeletonDetectionModel, map<int, int>> &JOINT_ATTENTION_START = {
        {SkeletonDetectionModel::OPENPOSE,    {
                                                      {0, 0},  // Nose
                                                      {1, 0},  // Neck
                                                      {2, 2},  // Right Shoulder
                                                      {3, 1},  // Right Elbow
                                                      {4, 0},  // Right Wrist
                                                      {5, 2},  // Left Shoulder
                                                      {6, 1},  // Left Elbow
                                                      {7, 0},  // Left Wrist
                                                      {8, 0},  // Mid Hip
                                                      {9, 2},  // Right Hip
                                                      {10, 1},  // Right Knee
                                                      {11, 0},  // Right Ankle
                                                      {12, 2},  // Left Hip
                                                      {13, 1},  // Left Knee
                                                      {14, 0},  // Left Ankle
                                                      {15, 1},  // Right Eye
                                                      {16, 1},  // Left Eye
                                                      {17, 2},  // Right Ear
                                                      {18, 2},  // Left Ear
                                                      {19, 3},  // Left Big Toe
                                                      {20, 3},  // Left Small Toe
                                                      {21, 3},  // Left Heel
                                                      {22, 3},  // Right Big Toe
                                                      {23, 3},  // Right Small Toe
                                                      {24, 3},  // Right Heel
                                              }},
        {SkeletonDetectionModel::OPENPOSE_18, {
                                                      {0, 0},
                                                      {1, 0},
                                                      {2, 0},
                                                      {3, 0},
                                                      {4, 0},
                                                      {5, 0},
                                                      {6, 0},
                                                      {7, 0},
                                                      {8, 0},
                                                      {9, 0},
                                                      {10, 0},
                                                      {11, 0},
                                                      {12, 0},
                                                      {13, 0},
                                                      {14, 0},
                                                      {15, 0},
                                                      {16, 0},
                                                      {17, 0},
                                              }},
        {SkeletonDetectionModel::OPENPOSE_15, {
                                                      {0, 0},
                                                      {1, 0},
                                                      {2, 0},
                                                      {3, 0},
                                                      {4, 0},
                                                      {5, 0},
                                                      {6, 0},
                                                      {7, 0},
                                                      {8, 0},
                                                      {9, 0},
                                                      {10, 0},
                                                      {11, 0},
                                                      {12, 0},
                                                      {13, 0},
                                                      {14, 0},
                                              }},
        {SkeletonDetectionModel::KINECT,      {
                                                      {0, 0},  // Nose
                                                      {1, 0},  // Neck
                                                      {2, 0},  // Low Spine
                                                      {3, 0},  // Mid Hip
                                                      {4, 3},  // Right Shoulder
                                                      {5, 2},  // Right Elbow
                                                      {6, 1},  // Right Wrist
                                                      {7, 0},  // Right Hand Tip
                                                      {8, 3},  // Left Shoulder
                                                      {9, 2},  // Left Elbow
                                                      {10, 1},  // Left Wrist
                                                      {11, 0},  // Left Hand Tip
                                                      {12, 3},  // Right Hip
                                                      {13, 1},  // Right Knee
                                                      {14, 2},  // Right Ankle
                                                      {15, 0},  // Right Foot Tip
                                                      {16, 3},  // Left Hip
                                                      {17, 1},  // Left Knee
                                                      {18, 2},  // Left Ankle
                                                      {19, 0},  // Left Foot Tip

                                              }},
};
#pragma clang diagnostic pop