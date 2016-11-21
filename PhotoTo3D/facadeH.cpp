﻿#include "facadeH.h"
#include "Utils.h"

cv::Mat generateFacadeH(int width, int height, int thickness, std::pair<int, int> range_NF, std::pair<int, int> range_NC, const std::vector<float>& params) {
	// #floors has to be at least 1 for this facade.
	if (range_NF.first < 1) range_NF.first = 1;

	// #columns has to be at least 6 for this facade.
	if (range_NC.first < 6) range_NC.first = 6;

	int NF = std::round(params[0] * (range_NF.second - range_NF.first) + range_NF.first);
	if (NF < range_NF.first) NF = range_NF.first;
	int NC = std::round(params[1] * (range_NC.second - range_NC.first) + range_NC.first);
	if (NC < range_NC.first) NC = range_NC.first;
	if (NC % 3 != 0) NC = std::round((float)NC / 3.0f) * 3;

	float BS = (float)width / (params[7] * 2 + params[8] * NC / 3 * 2 + params[9] * NC / 3) * params[7];
	float TW = (float)width / (params[7] * 2 + params[8] * NC / 3 * 2 + params[9] * NC / 3) * params[8];
	float TW2 = (float)width / (params[7] * 2 + params[8] * NC / 3 * 2 + params[9] * NC / 3) * params[9];
	float AH = (float)height / (params[10] + params[11] * NF + params[12]) * params[10];
	float FH = (float)height / (params[10] + params[11] * NF + params[12]) * params[11];
	float BH = (float)height / (params[10] + params[11] * NF + params[12]) * params[12];

	float WW = TW / (params[2] + params[4] * 2) * params[2];
	float WH = FH / (params[3] + params[5] + params[6]) * params[3];
	float WS = TW / (params[2] + params[4] * 2) * params[4];
	float WT = FH / (params[3] + params[5] + params[6]) * params[5];
	float WB = FH / (params[3] + params[5] + params[6]) * params[6];

	float WW2 = TW2 / (params[14] + params[13] *2) * params[14];
	float WS2 = TW2 / (params[14] + params[13] * 2) * params[13];
	float WH2 = FH / (params[15] + params[16] + params[17]) * params[16];
	float WT2 = FH / (params[15] + params[16] + params[17]) * params[15];
	float WB2 = FH / (params[15] + params[16] + params[17]) * params[17];

	return generateFacadeH(1, NF, NC, width, height, thickness, WW, WH, WS, WT, WB, BS, TW, TW2, AH, FH, BH, WS2, WW2, WT2, WH2, WB2);
}

cv::Mat generateRandomFacadeH(int width, int height, int thickness, std::pair<int, int> range_NF, std::pair<int, int> range_NC, std::vector<float>& params, int window_displacement, float window_prob) {
	// #floors has to be at least 1 for this facade.
	if (range_NF.first < 1) range_NF.first = 1;

	// #columns has to be at least 6 for this facade.
	if (range_NC.first < 6) range_NC.first = 6;

	///////////////////////////////////////////////////////////////////////////////////
	// パラメータを設定
	float ratio;

	int NF = utils::genRand(range_NF.first, range_NF.second + 1);
	int NC = utils::genRand(range_NC.first, range_NC.second + 1);
	if (NC % 3 != 0) NC = ((int)(NC / 3)) * 3;

	// ベースの高さ
	float BH = utils::genRand(0, 0.5);

	// 屋根の高さ
	float AH = utils::genRand(0, 0.5);

	// 各フロアの高さ
	float FH = utils::genRand(2.5, 4);

	// 各タイルの幅
	float TW = utils::genRand(2, 4);

	// 各タイルの幅
	float TW2;
	if (utils::genRand() < 0.5 && TW * 0.5 > 0.8) {
		TW2 = utils::genRand(0.8, TW * 0.5);
	}
	else {
		TW2 = utils::genRand(TW * 2, TW * 3);
	}

	// ビルの横マージン
	float BS = utils::genRand(0, 1);

	// 各フロアの窓上部から天井までの高さ
	float WT = utils::genRand(0.2, 1);

	// 各フロアの窓下部からフロア底面までの高さ
	float WB = utils::genRand(0.2, 1);

	// 各フロアの窓の高さ
	float WH = utils::genRand(1, 2.5);

	// 各フロアの各種高さをnormalize
	ratio = FH / (WT + WB + WH);
	WT *= ratio;
	WB *= ratio;
	WH *= ratio;

	// 中央の窓上部から天井までの高さ
	float WT2 = utils::genRand(0.2, 1);

	// 中央の窓下部からフロア底面までの高さ
	float WB2 = utils::genRand(0.2, 1);

	// 中央の窓の高さ
	float WH2 = utils::genRand(1, 2.5);

	// 中央の各種高さをnormalize
	ratio = FH / (WT2 + WB2 + WH2);
	WT2 *= ratio;
	WB2 *= ratio;
	WH2 *= ratio;

	// 各フロアの窓の横マージン
	float WS = utils::genRand(0.2, 1);

	// 各フロアの窓の幅
	float WW = utils::genRand(0.5, 2.5);

	// 各フロアの各種幅をnormalize
	ratio = TW / (WS * 2 + WW);
	WS *= ratio;
	WW *= ratio;

	// 中央の窓の横のマージン
	float WS2 = utils::genRand(0.2, 1);

	// 中央の窓の幅
	float WW2 = utils::genRand(0.5, 2.5);

	// 中央の各種幅をnormalize
	ratio = TW2 / (WS2 * 2 + WW2);
	WS2 *= ratio;
	WW2 *= ratio;

	// すべてを画像のサイズにnormalize
	ratio = (float)height / (BH + AH + FH * NF);
	BH *= ratio;
	FH *= ratio;
	WT *= ratio;
	WB *= ratio;
	WH *= ratio;
	WT2 *= ratio;
	WH2 *= ratio;
	WB2 *= ratio;
	AH *= ratio;
	ratio = (float)width / (BS * 2 + TW * NC / 3 * 2 + TW2 * NC / 3);
	BS *= ratio;
	WS *= ratio;
	WW *= ratio;
	TW *= ratio;
	TW2 *= ratio;
	WS2 *= ratio;
	WW2 *= ratio;

	///////////////////////////////////////////////////////////////////////////////////
	// パラメータ値を格納
	params.push_back((float)(NF - range_NF.first) / (float)(range_NF.second - range_NF.first));
	params.push_back((float)(NC - range_NC.first) / (float)(range_NC.second - range_NC.first));
	params.push_back(WW / TW);
	params.push_back(WH / FH);
	params.push_back(WS / TW);
	params.push_back(WT / FH);
	params.push_back(WB / FH);
	params.push_back(BS / width);
	params.push_back(TW / width);
	params.push_back(TW2 / width);
	params.push_back(AH / height);
	params.push_back(FH / height);
	params.push_back(BH / height);
	params.push_back(WS2 / TW2);
	params.push_back(WW2 / TW2);
	params.push_back(WT2 / FH);
	params.push_back(WH2 / FH);
	params.push_back(WB2 / FH);

	return generateFacadeH(NF, NC, width, height, thickness, WW, WH, WS, WT, WB, BS, TW, TW2, AH, FH, BH, WS2, WW2, WT2, WH2, WB2, window_displacement, window_prob);
}

cv::Mat generateFacadeH(float scale, int NF, int NC, int width, int height, int thickness, float WW, float WH, float WS, float WT, float WB, float BS, float TW, float TW2, float AH, float FH, float BH, float WS2, float WW2, float WT2, float WH2, float WB2, int window_displacement, float window_prob) {
	cv::Mat result(height * scale, width * scale, CV_8UC3, cv::Scalar(255, 255, 255));

	// 窓を描画
	for (int i = 0; i < NF; ++i) {
		for (int j = 0; j < NC / 3; ++j) {
			for (int k = 0; k < 3; ++k) {
				int x1, y1, x2, y2;
				if (k == 0) {
					x1 = (BS + (TW * 2 + TW2) * j + WS) * scale;
					y1 = (height - BH - FH * i - WB - WH) * scale;
					x2 = (BS + (TW * 2 + TW2) * j + WS + WW) * scale;
					y2 = (height - BH - FH * i - WB) * scale;
				}
				else if (k == 1) {
					x1 = (BS + (TW * 2 + TW2) * j + TW + WS2) * scale;
					y1 = (height - BH - FH * i - WB2 - WH2) * scale;
					x2 = (BS + (TW * 2 + TW2) * j + TW + WS2 + WW2) * scale;
					y2 = (height - BH - FH * i - WB2) * scale;
				}
				else {
					x1 = (BS + (TW * 2 + TW2) * j + TW + TW2 + WS) * scale;
					y1 = (height - BH - FH * i - WB - WH) * scale;
					x2 = (BS + (TW * 2 + TW2) * j + TW + TW2 + WS + WW) * scale;
					y2 = (height - BH - FH * i - WB) * scale;
				}

				if (window_displacement > 0) {
					x1 += utils::genRand(-window_displacement, window_displacement + 1);
					y1 += utils::genRand(-window_displacement, window_displacement + 1);
					x2 += utils::genRand(-window_displacement, window_displacement + 1);
					y2 += utils::genRand(-window_displacement, window_displacement + 1);
				}

				if (utils::genRand() < window_prob) {
					cv::rectangle(result, cv::Point(x1, y1), cv::Point(x2, y2), cv::Scalar(0, 0, 0), thickness);
				}
			}
		}
	}

	return result;
}