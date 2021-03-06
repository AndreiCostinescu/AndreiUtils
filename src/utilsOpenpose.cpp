//
// Created by Andrei on 12.11.20.
//

#include <AndreiUtils/utilsOpenpose.h>
#include <openpose/flags.hpp>
#include <iostream>

using namespace op;
using namespace std;

void configureOpenposeWrapper(op::WrapperT<op::Datum> &opWrapper, bool trackPeople, bool verbose) {
    try {
        // Configuring OpenPose

        // logging_level
        op::checkBool(0 <= FLAGS_logging_level && FLAGS_logging_level <= 255, "Wrong logging_level value.",
                      __LINE__, __FUNCTION__, __FILE__);
        op::ConfigureLog::setPriorityThreshold((op::Priority) FLAGS_logging_level);
        op::Profiler::setDefaultX(FLAGS_profile_speed);

        // Applying user defined configuration - GFlags to program variables
        // outputSize
        const auto outputSize = op::flagsToPoint(op::String(FLAGS_output_resolution), "-1x-1");
        // netInputSize
        const auto netInputSize = op::flagsToPoint(op::String(FLAGS_net_resolution), "-1x368");
        // const auto netInputSize = op::flagsToPoint(op::String(FLAGS_net_resolution), "-1x160");
        if (verbose) {
            cout << "Openpose network input size: " << netInputSize << R"( (change "-1x160" or "-1x368"))" << endl;
        }
        // faceNetInputSize
        const auto faceNetInputSize = op::flagsToPoint(op::String(FLAGS_face_net_resolution),
                                                       "368x368 (multiples of 16)");
        // handNetInputSize
        const auto handNetInputSize = op::flagsToPoint(op::String(FLAGS_hand_net_resolution),
                                                       "368x368 (multiples of 16)");
        // poseMode
        const auto poseMode = op::flagsToPoseMode(FLAGS_body);
        // poseModel
        const auto poseModel = op::flagsToPoseModel(op::String(FLAGS_model_pose));
        // JSON saving
        if (!FLAGS_write_keypoint.empty()) {
            op::opLog("Flag `write_keypoint` is deprecated and will eventually be removed. Please, use `write_json`"
                      " instead.", op::Priority::Max);
        }
        // keypointScaleMode
        const auto keypointScaleMode = op::flagsToScaleMode(FLAGS_keypoint_scale);
        // heatmaps to add
        const auto heatMapTypes = op::flagsToHeatMaps(FLAGS_heatmaps_add_parts, FLAGS_heatmaps_add_bkg,
                                                      FLAGS_heatmaps_add_PAFs);
        const auto heatMapScaleMode = op::flagsToHeatMapScaleMode(FLAGS_heatmaps_scale);
        // >1 camera view?
        const auto multipleView = (FLAGS_3d || FLAGS_3d_views > 1);
        // Face and hand detectors
        const auto faceDetector = op::flagsToDetector(FLAGS_face_detector);
        const auto handDetector = op::flagsToDetector(FLAGS_hand_detector);
        // Enabling Google Logging
        const bool enableGoogleLogging = true;

        // Pose configuration (use WrapperStructPose{} for default and recommended configuration)
        const op::WrapperStructPose wrapperStructPose{
                poseMode, netInputSize, outputSize, keypointScaleMode, FLAGS_num_gpu, FLAGS_num_gpu_start,
                FLAGS_scale_number, (float) FLAGS_scale_gap, op::flagsToRenderMode(FLAGS_render_pose, multipleView),
                poseModel, !FLAGS_disable_blending, (float) FLAGS_alpha_pose, (float) FLAGS_alpha_heatmap,
                FLAGS_part_to_show, op::String(FLAGS_model_folder), heatMapTypes, heatMapScaleMode,
                FLAGS_part_candidates, (float) FLAGS_render_threshold, FLAGS_number_people_max,
                FLAGS_maximize_positives, FLAGS_fps_max, op::String(FLAGS_prototxt_path),
                op::String(FLAGS_caffemodel_path), (float) FLAGS_upsampling_ratio, enableGoogleLogging};
        opWrapper.configure(wrapperStructPose);
        // Face configuration (use op::WrapperStructFace{} to disable it)
        const op::WrapperStructFace wrapperStructFace{
                FLAGS_face, faceDetector, faceNetInputSize,
                op::flagsToRenderMode(FLAGS_face_render, multipleView, FLAGS_render_pose),
                (float) FLAGS_face_alpha_pose, (float) FLAGS_face_alpha_heatmap,
                (float) FLAGS_face_render_threshold};
        opWrapper.configure(wrapperStructFace);
        // Hand configuration (use op::WrapperStructHand{} to disable it)
        const op::WrapperStructHand wrapperStructHand{
                FLAGS_hand, handDetector, handNetInputSize, FLAGS_hand_scale_number, (float) FLAGS_hand_scale_range,
                op::flagsToRenderMode(FLAGS_hand_render, multipleView, FLAGS_render_pose),
                (float) FLAGS_hand_alpha_pose, (float) FLAGS_hand_alpha_heatmap,
                (float) FLAGS_hand_render_threshold};
        opWrapper.configure(wrapperStructHand);
        // Extra functionality configuration (use op::WrapperStructExtra{} to disable it)
        const int tracking = (trackPeople) ? 0 : FLAGS_tracking;
        const op::WrapperStructExtra wrapperStructExtra{
                FLAGS_3d, FLAGS_3d_min_views, FLAGS_identification, tracking, FLAGS_ik_threads};
        opWrapper.configure(wrapperStructExtra);
        // Output (comment or use default argument to disable any output)
        const op::WrapperStructOutput wrapperStructOutput{
                FLAGS_cli_verbose, op::String(FLAGS_write_keypoint),
                op::stringToDataFormat(FLAGS_write_keypoint_format), op::String(FLAGS_write_json),
                op::String(FLAGS_write_coco_json), FLAGS_write_coco_json_variants, FLAGS_write_coco_json_variant,
                op::String(FLAGS_write_images), op::String(FLAGS_write_images_format),
                op::String(FLAGS_write_video),
                FLAGS_write_video_fps, FLAGS_write_video_with_audio, op::String(FLAGS_write_heatmaps),
                op::String(FLAGS_write_heatmaps_format), op::String(FLAGS_write_video_3d),
                op::String(FLAGS_write_video_adam), op::String(FLAGS_write_bvh), op::String(FLAGS_udp_host),
                op::String(FLAGS_udp_port)};
        opWrapper.configure(wrapperStructOutput);
        // No GUI. Equivalent to: opWrapper.configure(op::WrapperStructGui{});
        // Set to single-thread (for sequential processing and/or debugging and/or reducing latency)
        if (FLAGS_disable_multi_thread) {
            opWrapper.disableMultiThreading();
        }
    }
    catch (const std::exception &e) {
        op::error(e.what(), __LINE__, __FUNCTION__, __FILE__);
    }
}
