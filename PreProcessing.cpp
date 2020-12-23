#include "PreProcessing.h"

PreProcessing::PreProcessing() {
}

Mat PreProcessing::pre_process(Mat frame, bool lower_res) {
	Mat cropped;
	if (frame.cols == 1920 && frame.rows == 1080) {
		if (lower_res) {
			resize(frame, frame, Size(), 0.6667, 0.667, INTER_AREA);
			//Mat crop_rect(frame, Rect(2, 0, 1908, 857));
			Mat crop_rect(frame, Rect(0, 125, 1279, 450));
			crop_rect.copyTo(cropped);
		}else{
			Mat crop_rect(frame, Rect(0, 198, 1872, 662));
			crop_rect.copyTo(cropped);
		}

	} else {
		Mat crop_rect(frame, Rect(35, 90, 1180, 630));
		crop_rect.copyTo(cropped);
	}

	Mat blurred_frame;
	medianBlur(cropped, blurred_frame, 5);
	return blurred_frame;
}