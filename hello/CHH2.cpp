
#include "CHH2.h"
#include "qdebug.h"

void CHH2::PengShang_Camera3(const HObject& ho_ImageEmphasize, const HObject& ho_TileImage, HTuple hv_WindowHandle,
	HTuple *hv_IsBad)
{
	try{


		// Local iconic variables
		HObject  ho_ImageOpening, ho_ImageClosing, ho_Regions;
		HObject  ho_RegionOpening, ho_ConnectedRegions, ho_SelectedRegions;
		HObject  ho_Rectangle, ho_RegionUnion, ho_RegionIntersection;
		HObject  ho_ImageReduced, ho_Bad, ho_RegionOpening1, ho_ConnectedRegions1;
		HObject  ho_RegionTrans, ho_SelectedRegions1, ho_BadObjects;
		HObject  ho_ObjectSelected, ho_RegionDilation, ho_RegionUnion1;
		HObject  ho_RegionClosing, ho_ConnectedRegions2, ho_Bad2Objects;
		HObject  ho_RegionDilation2;

		// Local control variables
		HTuple  hv_WIDTH, hv_Number, hv_Col2s, hv_Col2;
		HTuple  hv_Col1, hv_Number1, hv_GrayMeans, hv_Sgn, hv_Indices;
		HTuple  hv_badNum, hv_Index, hv_BadNumber, hv_Number2;

		//****************************
		//**
		//****************************

		(*hv_IsBad) = 0;
		hv_WIDTH = 870;

		GrayOpeningRect(ho_ImageEmphasize, &ho_ImageOpening, 7, 7);
		GrayClosingRect(ho_ImageOpening, &ho_ImageClosing, 21, 21);

		Threshold(ho_ImageClosing, &ho_Regions, 20, 255);
		OpeningCircle(ho_Regions, &ho_RegionOpening, 15.5);
		Connection(ho_RegionOpening, &ho_ConnectedRegions);
		SelectShape(ho_ConnectedRegions, &ho_SelectedRegions, (HTuple("height").Append("column2")),
			"and", (HTuple(400).Append(0)), (HTuple(800).Append(1800)));

		CountObj(ho_SelectedRegions, &hv_Number);
		RegionFeatures(ho_SelectedRegions, "column2", &hv_Col2s);
		TupleMean(hv_Col2s, &hv_Col2);
		hv_Col1 = hv_Col2 - hv_WIDTH;
		GenRectangle1(&ho_Rectangle, 0, hv_Col1, 9999, hv_Col2);
		Union1(ho_SelectedRegions, &ho_RegionUnion);
		Intersection(ho_Rectangle, ho_RegionUnion, &ho_RegionIntersection);

		ReduceDomain(ho_ImageClosing, ho_RegionIntersection, &ho_ImageReduced);
		Threshold(ho_ImageReduced, &ho_Bad, 20, 200);
		OpeningCircle(ho_Bad, &ho_RegionOpening1, 8.5);
		Connection(ho_RegionOpening1, &ho_ConnectedRegions1);
		ShapeTrans(ho_ConnectedRegions1, &ho_RegionTrans, "convex");
		SelectShape(ho_RegionTrans, &ho_SelectedRegions1, "area", "and", 800, 999999);
		CountObj(ho_SelectedRegions1, &hv_Number1);
		GrayFeatures(ho_SelectedRegions1, ho_TileImage, "mean", &hv_GrayMeans);
		TupleSgn(hv_GrayMeans - 100, &hv_Sgn);
		TupleFind(hv_Sgn, -1, &hv_Indices);
		hv_badNum = hv_Indices.TupleLength();
		GenEmptyObj(&ho_BadObjects);
		if (0 != (HTuple(hv_badNum != 0).TupleAnd(hv_Indices != -1)))
		{
			{
				HTuple end_val36 = hv_badNum - 1;
				HTuple step_val36 = 1;
				for (hv_Index = 0; hv_Index.Continue(end_val36, step_val36); hv_Index += step_val36)
				{
					SelectObj(ho_SelectedRegions1, &ho_ObjectSelected, HTuple(hv_Indices[hv_Index]) + 1);
					ConcatObj(ho_BadObjects, ho_ObjectSelected, &ho_BadObjects);
				}
			}
		}
		CountObj(ho_BadObjects, &hv_BadNumber);
		//if (HDevWindowStack::IsOpen())
		DispObj(ho_TileImage, hv_WindowHandle);
		//if (HDevWindowStack::IsOpen())
			SetDraw(hv_WindowHandle, "margin");
		//if (HDevWindowStack::IsOpen())
			SetColor(hv_WindowHandle, "red");
		//if (HDevWindowStack::IsOpen())
			SetLineWidth(hv_WindowHandle, 2);

		if (0 != (hv_BadNumber>0))
		{
			(*hv_IsBad) = 1;
			DilationCircle(ho_BadObjects, &ho_RegionDilation, 8.5);
			//if (HDevWindowStack::IsOpen())
				DispObj(ho_RegionDilation, hv_WindowHandle);
		}

		Union1(ho_SelectedRegions1, &ho_RegionUnion1);
		ClosingCircle(ho_RegionUnion1, &ho_RegionClosing, 5.5);
		Connection(ho_RegionClosing, &ho_ConnectedRegions2);
		SelectShape(ho_ConnectedRegions2, &ho_SelectedRegions1, (HTuple("area").Append("column2")),
			"and", HTuple(1200).TupleConcat(hv_Col2 - 100), HTuple(999999).TupleConcat(hv_Col2 + 50));
		GrayFeatures(ho_SelectedRegions1, ho_TileImage, "mean", &hv_GrayMeans);
		TupleSgn(hv_GrayMeans - 130, &hv_Sgn);
		TupleFind(hv_Sgn, -1, &hv_Indices);
		hv_badNum = hv_Indices.TupleLength();
		GenEmptyObj(&ho_Bad2Objects);
		if (0 != (HTuple(hv_badNum != 0).TupleAnd(hv_Indices != -1)))
		{
			{
				HTuple end_val63 = hv_badNum - 1;
				HTuple step_val63 = 1;
				for (hv_Index = 0; hv_Index.Continue(end_val63, step_val63); hv_Index += step_val63)
				{
					SelectObj(ho_SelectedRegions1, &ho_ObjectSelected, HTuple(hv_Indices[hv_Index]) + 1);
					ConcatObj(ho_Bad2Objects, ho_ObjectSelected, &ho_Bad2Objects);
				}
			}
		}

		CountObj(ho_Bad2Objects, &hv_Number2);
		if (0 != (hv_Number2>0))
		{
			(*hv_IsBad) = 1;
			DilationCircle(ho_Bad2Objects, &ho_RegionDilation2, 8.5);
			//if (HDevWindowStack::IsOpen())
				DispObj(ho_RegionDilation2, hv_WindowHandle);
		}

		return;
	}
	catch (HException& except)
	{
		qDebug() << "error: " << except.ErrorMessage().Text();
		(*hv_IsBad) = 1;
		CHH::disp_message(hv_WindowHandle, "PengShang_Camera3 Code Error.", "image", 400, 200, "red", "true");
		return;
	}
}

void CHH2::BottomAndTop_Camera4(HObject ho_ImageEmphasize, HObject ho_TileImage,
	HTuple hv_WindowHandle, HTuple hv_Model, HTuple *hv_IsBad)
{
	try{
		// Local iconic variables
		HObject  ho_Regions, ho_RegionOpening, ho_ConnectedRegions;
		HObject  ho_SelectedRegions, ho_RegionTrans1, ho_Rectangle;
		HObject  ho_ImageReduced, ho_ImageClosing, ho_Regions1, ho_RegionFillUp1;
		HObject  ho_RegionOpening1, ho_RegionClosing, ho_ConnectedRegions1;
		HObject  ho_PieceRegions, ho_RegionFillUp, ho_RegionTrans;
		HObject  ho_ConnectedTrans, ho_BadObjects1, ho_BadObjects2;
		HObject  ho_BadObjects3, ho_ObjectTran, ho_ObjectSelected;
		HObject  ho_RegionDifference, ho_Rectangle1, ho_RegionIntersection;
		HObject  ho_ImageReduced2, ho_ImagePart, ho_RegionOpening4;
		HObject  ho_RegionOpening5, ho_ConnectedRegions4, ho_SelectedRegions5;
		HObject  ho_ImageReduced1, ho_BadRegion, ho_BadRegions;

		// Local control variables
		HTuple  hv_WIDTH, hv_col2, hv_PieceNum, hv_MLPHandle;
		HTuple  hv_i, hv_row1, hv_FeatureVector, hv_FoundClassIDs;
		HTuple  hv_Confidence, hv_grayMeanValue, hv_GrayThreshold;
		HTuple  hv_Sgn, hv_Indices, hv_badNum, hv_Index, hv_Value;
		HTuple  hv_BabNum;

		//***********************************
		//**Step1:find column1 of bottom light Rectangel.
		//**Step2:Gen_Rectangle1 and ReduceImage.
		//**Step3:find 18 Pieces.
		//***********************************
		(*hv_IsBad) = 0;
		hv_WIDTH = 950;
		Threshold(ho_ImageEmphasize, &ho_Regions, 27, 255);
		OpeningRectangle1(ho_Regions, &ho_RegionOpening, 1, 130);
		Connection(ho_RegionOpening, &ho_ConnectedRegions);
		SelectShape(ho_ConnectedRegions, &ho_SelectedRegions, "height", "and", 5000, 99999);
		ShapeTrans(ho_SelectedRegions, &ho_RegionTrans1, "rectangle1");
		RegionFeatures(ho_SelectedRegions, "column1", &hv_col2);
		GenRectangle1(&ho_Rectangle, 0, hv_col2 - hv_WIDTH, 9999, hv_col2 + 30);

		ReduceDomain(ho_ImageEmphasize, ho_Rectangle, &ho_ImageReduced);
		GrayClosingRect(ho_ImageReduced, &ho_ImageClosing, 7, 7);

		Threshold(ho_ImageClosing, &ho_Regions1, 200, 255);
		FillUp(ho_Regions1, &ho_RegionFillUp1);
		OpeningCircle(ho_RegionFillUp1, &ho_RegionOpening1, 3.5);
		ClosingCircle(ho_RegionOpening1, &ho_RegionClosing, 5.5);

		Connection(ho_RegionClosing, &ho_ConnectedRegions1);
		SelectShape(ho_ConnectedRegions1, &ho_PieceRegions, "height", "and", 300, 5000);

		CountObj(ho_PieceRegions, &hv_PieceNum);
		if (0 != (hv_PieceNum != 18))
		{
			return;

		}
		else
		{

			FillUp(ho_PieceRegions, &ho_RegionFillUp);
			ShapeTrans(ho_RegionFillUp, &ho_RegionTrans, "rectangle2");

			
			DispObj(ho_TileImage, hv_WindowHandle);
			SetDraw(hv_WindowHandle, "margin");
			DispObj(ho_RegionTrans, hv_WindowHandle);

			Connection(ho_RegionTrans, &ho_ConnectedTrans);
			GenEmptyObj(&ho_BadObjects1);
			GenEmptyObj(&ho_BadObjects2);
			GenEmptyObj(&ho_BadObjects3);

			ReadClassMlp(hv_Model, &hv_MLPHandle);

			{
				HTuple end_val46 = hv_PieceNum;
				HTuple step_val46 = 1;
				for (hv_i = 1; hv_i.Continue(end_val46, step_val46); hv_i += step_val46)
				{
					SelectObj(ho_ConnectedTrans, &ho_ObjectTran, hv_i);
					SelectObj(ho_RegionFillUp, &ho_ObjectSelected, hv_i);
					RegionFeatures(ho_ObjectSelected, "column2", &hv_col2);
					Difference(ho_ObjectTran, ho_ObjectSelected, &ho_RegionDifference);

					//*****MLP***
					RegionFeatures(ho_ObjectSelected, "row1", &hv_row1);
					GenRectangle1(&ho_Rectangle1, hv_row1, hv_col2 - 200, hv_row1 + 500, hv_col2);
					Intersection(ho_Rectangle1, ho_ObjectTran, &ho_RegionIntersection);
					ReduceDomain(ho_TileImage, ho_RegionIntersection, &ho_ImageReduced2);
					CropDomain(ho_ImageReduced2, &ho_ImagePart);

					CHH2::Gen_features(ho_ImagePart, &hv_FeatureVector);
					ClassifyClassMlp(hv_MLPHandle, hv_FeatureVector, 1, &hv_FoundClassIDs, &hv_Confidence);
					if (0 != (hv_FoundClassIDs == 1))
					{
						ConcatObj(ho_BadObjects1, ho_RegionIntersection, &ho_BadObjects1);
					}


					//**************底部宽大的黑条************
					//opening_circle (RegionDifference, RegionOpening2, 8.5)
					//closing_circle (RegionOpening2, RegionClosing2, 5.5)
					//connection (RegionClosing2, ConnectedRegions2)
					//select_shape (ConnectedRegions2, BadRegions1, ['width','column2','area','column1'], 'and', [20,col2-50,1100,col2-200], [99999,col2+30,99999,col2-15])
					//concat_obj (BadObjects1, BadRegions1, BadObjects1)

					//**************底部细长的黑条************
					OpeningRectangle1(ho_RegionDifference, &ho_RegionOpening4, 1, 15);
					OpeningRectangle1(ho_RegionOpening4, &ho_RegionOpening5, 10, 1);

					Connection(ho_RegionOpening5, &ho_ConnectedRegions4);
					SelectShape(ho_ConnectedRegions4, &ho_SelectedRegions5, (HTuple("height").Append("column2")),
						"and", HTuple(80).TupleConcat(hv_col2 - 50), HTuple(99999).TupleConcat(hv_col2 + 30));

					GrayFeatures(ho_SelectedRegions5, ho_TileImage, "mean", &hv_grayMeanValue);

					hv_GrayThreshold = 30;
					TupleSgn(hv_grayMeanValue - hv_GrayThreshold, &hv_Sgn);
					TupleFind(hv_Sgn, -1, &hv_Indices);

					hv_badNum = hv_Indices.TupleLength();
					if (0 != (HTuple(hv_badNum != 0).TupleAnd(hv_Indices != -1)))
					{
						{
							HTuple end_val90 = hv_badNum - 1;
							HTuple step_val90 = 1;
							for (hv_Index = 0; hv_Index.Continue(end_val90, step_val90); hv_Index += step_val90)
							{
								SelectObj(ho_SelectedRegions5, &ho_ObjectSelected, HTuple(hv_Indices[hv_Index]) + 1);
								ConcatObj(ho_BadObjects2, ho_ObjectSelected, &ho_BadObjects2);
							}
						}
					}

					//**************黑色长条************
					ReduceDomain(ho_ImageClosing, ho_ObjectSelected, &ho_ImageReduced1);
					Threshold(ho_ImageReduced1, &ho_BadRegion, 0, 100);
					Connection(ho_BadRegion, &ho_BadRegion);
					RegionFeatures(ho_BadRegion, "outer_radius", &hv_Value);
					SelectShape(ho_BadRegion, &ho_BadRegions, ((HTuple("area").Append("outer_radius")).Append("orientation")),
						"and", (HTuple(500).Append(35)).TupleConcat(HTuple(20).TupleRad()), (HTuple(99999).Append(99999)).TupleConcat(HTuple(70).TupleRad()));
					ConcatObj(ho_BadObjects3, ho_BadRegions, &ho_BadObjects3);
				}
			}

			DispObj(ho_TileImage, hv_WindowHandle);
			SetColor(hv_WindowHandle, "red");
			SetDraw(hv_WindowHandle, "margin");
			SetLineWidth(hv_WindowHandle, 2);
			CountObj(ho_BadObjects1, &hv_BabNum);
			if (0 != (hv_BabNum>0))
			{
				(*hv_IsBad) = 1;
				DilationCircle(ho_BadObjects1, &ho_BadObjects1, 8.5);
				DispObj(ho_BadObjects1, hv_WindowHandle);
			}

			CountObj(ho_BadObjects2, &hv_BabNum);
			if (0 != (hv_BabNum>0))
			{
				(*hv_IsBad) = 1;
				DilationCircle(ho_BadObjects2, &ho_BadObjects2, 8.5);
				DispObj(ho_BadObjects2, hv_WindowHandle);
			}

			CountObj(ho_BadObjects3, &hv_BabNum);
			if (0 != (hv_BabNum>0))
			{
				(*hv_IsBad) = 1;
				DilationCircle(ho_BadObjects3, &ho_BadObjects3, 8.5);
				DispObj(ho_BadObjects3, hv_WindowHandle);
			}

		}
		return;
	}
catch (HException& except)
	{
		qDebug() << "error: " << except.ErrorMessage().Text();
		(*hv_IsBad) = 1;
		CHH::disp_message(hv_WindowHandle, "BottomAndTop_Camera4 Code Error.", "image", 400, 200, "red", "true");
		return;
	}
}

void CHH2::Gen_features(HObject ho_Image, HTuple *hv_FeatureVector)
{

	// Local iconic variables
	HObject  ho_Zoomed1, ho_Zoomed2;

	// Local control variables
	HTuple  hv_Width, hv_Height;

	(*hv_FeatureVector) = HTuple();
	GetImageSize(ho_Image, &hv_Width, &hv_Height);
	//Compute features.
	CHH2::Gen_Sobel_features(ho_Image, (*hv_FeatureVector), &(*hv_FeatureVector));
	//Downscale the image (image pyramid) and compute features.
	ZoomImageFactor(ho_Image, &ho_Zoomed1, 0.5, 0.5, "constant");
	CHH2::Gen_Sobel_features(ho_Zoomed1, (*hv_FeatureVector), &(*hv_FeatureVector));
	//Uncomment lines to use further pyramid levels:
	ZoomImageFactor(ho_Zoomed1, &ho_Zoomed2, 0.5, 0.5, "constant");
	//gen_sobel_features (Zoomed2, FeatureVector, FeatureVector)
	//zoom_image_factor (Zoomed2, Zoomed3, 0.5, 0.5, 'constant')
	//gen_sobel_features (Zoomed3, FeatureVector, FeatureVector)
	//zoom_image_factor (Zoomed3, Zoomed4, 0.5, 0.5, 'constant')
	//gen_sobel_features (Zoomed4, FeatureVector, FeatureVector)
	(*hv_FeatureVector) = ((*hv_FeatureVector).TupleConcat(hv_Width)).TupleConcat(hv_Height);
	(*hv_FeatureVector) = (*hv_FeatureVector).TupleReal();
	return;

}

void CHH2::Gen_Sobel_features(HObject ho_Image, HTuple hv_Features, HTuple *hv_FeaturesExtended)
{

	// Local iconic variables
	HObject  ho_EdgeAmplitude;

	// Local control variables
	HTuple  hv_Energy, hv_Correlation, hv_Homogeneity;
	HTuple  hv_Contrast, hv_AbsoluteHistoEdgeAmplitude;

	//Coocurrence matrix for 0 deg and 90 deg:
	/*CoocFeatureImage(ho_Image, ho_Image, 6, 0, &hv_Energy, &hv_Correlation, &hv_Homogeneity,
		&hv_Contrast);*/
	CoocFeatureImage(ho_Image, ho_Image, 6, 90, &hv_Energy, &hv_Correlation, &hv_Homogeneity,
		&hv_Contrast);
	//Absolute histogram of edge amplitudes:
	SobelAmp(ho_Image, &ho_EdgeAmplitude, "sum_abs", 3);
	GrayHistoAbs(ho_EdgeAmplitude, ho_EdgeAmplitude, 8, &hv_AbsoluteHistoEdgeAmplitude);
	//Entropy and anisotropy:
	//entropy_gray (Image, Image, Entropy, Anisotropy)
	//Absolute histogram of gray values:
	//gray_histo_abs (Image, Image, 8, AbsoluteHistoImage)
	//Add features to feature vector:
	(*hv_FeaturesExtended).Clear();
	(*hv_FeaturesExtended).Append(hv_Features);
	(*hv_FeaturesExtended).Append(hv_Energy);
	(*hv_FeaturesExtended).Append(hv_Correlation);
	(*hv_FeaturesExtended).Append(hv_Homogeneity);
	(*hv_FeaturesExtended).Append(hv_Contrast);
	(*hv_FeaturesExtended) = (*hv_FeaturesExtended).TupleConcat(hv_AbsoluteHistoEdgeAmplitude);
	//FeaturesExtended := [FeaturesExtended,Entropy,Anisotropy]
	//FeaturesExtended := [FeaturesExtended,AbsoluteHistoImage]

	return;
}

void CHH2::PengShang2_Camera3(HObject ho_ImageEmphasize, HObject ho_TileImage, HTuple hv_ModelHandle,
	HTuple hv_WindowHandle, HTuple *hv_IsBad)
{

	// Local iconic variables
	HObject  ho_ImageOpening, ho_ImageClosing, ho_Regions;
	HObject  ho_RegionOpening, ho_ConnectedRegions, ho_SelectedRegions;
	HObject  ho_RegionClosing, ho_RegionFillUp, ho_Rectangle;
	HObject  ho_RegionUnion, ho_RegionIntersection, ho_RegionErosion;
	HObject  ho_ImageReduced, ho_Region, ho_ConnectedRegions1;
	HObject  ho_BadRegions1, ho_RegionUnion1, ho_RegionFillUp1;
	HObject  ho_ConnectedRegions2, ho_RegionTrans, ho_BadObjects;
	HObject  ho_ObjectSelected, ho_CropImage;

	// Local control variables
	HTuple  hv_WIDTH, hv_SaveImagePath, hv_Number;
	HTuple  hv_Col2s, hv_Col2, hv_Col1, hv_Index, hv_Value;
	HTuple  hv_FeatureVector, hv_FoundClassIDs, hv_Confidence;
	HTuple  hv_Num;

	//****************************
	//**
	//****************************

	(*hv_IsBad) = 0;
	hv_WIDTH = 870;
	hv_SaveImagePath = "f:/mlp/auto3/";

	GrayOpeningRect(ho_ImageEmphasize, &ho_ImageOpening, 7, 7);
	GrayClosingRect(ho_ImageOpening, &ho_ImageClosing, 5, 5);

	Threshold(ho_ImageClosing, &ho_Regions, 20, 255);
	OpeningCircle(ho_Regions, &ho_RegionOpening, 15.5);
	Connection(ho_RegionOpening, &ho_ConnectedRegions);
	SelectShape(ho_ConnectedRegions, &ho_SelectedRegions, (HTuple("height").Append("column2")),
		"and", (HTuple(400).Append(0)), (HTuple(800).Append(1800)));

	CountObj(ho_SelectedRegions, &hv_Number);
	ClosingCircle(ho_SelectedRegions, &ho_RegionClosing, 10.5);
	FillUp(ho_RegionClosing, &ho_RegionFillUp);
	RegionFeatures(ho_RegionFillUp, "column2", &hv_Col2s);
	TupleMean(hv_Col2s, &hv_Col2);
	hv_Col1 = hv_Col2 - hv_WIDTH;
	GenRectangle1(&ho_Rectangle, 0, hv_Col1, 9999, hv_Col2);
	Union1(ho_RegionFillUp, &ho_RegionUnion);
	Intersection(ho_Rectangle, ho_RegionUnion, &ho_RegionIntersection);

	ErosionCircle(ho_RegionIntersection, &ho_RegionErosion, 6.5);

	ReduceDomain(ho_ImageClosing, ho_RegionErosion, &ho_ImageReduced);
	Threshold(ho_ImageReduced, &ho_Region, 0, 200);
	ClosingCircle(ho_Region, &ho_RegionClosing, 5.5);
	Connection(ho_RegionClosing, &ho_ConnectedRegions1);
	SelectShape(ho_ConnectedRegions1, &ho_BadRegions1, "area", "and", 700, 99999);
	Union1(ho_BadRegions1, &ho_RegionUnion1);
	FillUp(ho_RegionUnion1, &ho_RegionFillUp1);
	Connection(ho_RegionFillUp1, &ho_ConnectedRegions2);
	ShapeTrans(ho_ConnectedRegions2, &ho_RegionTrans, "rectangle1");
	CountObj(ho_RegionTrans, &hv_Number);
	GenEmptyObj(&ho_BadObjects);
	{
		HTuple end_val39 = hv_Number;
		HTuple step_val39 = 1;
		for (hv_Index = 1; hv_Index.Continue(end_val39, step_val39); hv_Index += step_val39)
		{
			SelectObj(ho_RegionTrans, &ho_ObjectSelected, hv_Index);
			RegionFeatures(ho_ObjectSelected, (((HTuple("row1").Append("column1")).Append("width")).Append("height")),
				&hv_Value);

			CropPart(ho_TileImage, &ho_CropImage, HTuple(hv_Value[0]), HTuple(hv_Value[1]),
				HTuple(hv_Value[2]), HTuple(hv_Value[3]));
			//write_image (CropImage, 'tiff', 0, SaveImagePath+Index)

			CHH2::Gen_features(ho_CropImage, &hv_FeatureVector);
			ClassifyClassMlp(hv_ModelHandle, hv_FeatureVector, 1, &hv_FoundClassIDs, &hv_Confidence);
			if (0 != (hv_FoundClassIDs == 1))
			{
				//WriteImage(ho_CropImage, "tiff", 0, hv_SaveImagePath + hv_Index);
				ConcatObj(ho_BadObjects, ho_ObjectSelected, &ho_BadObjects);
			}

		}
	}

	//if (HDevWindowStack::IsOpen())
		DispObj(ho_TileImage, hv_WindowHandle);
	//if (HDevWindowStack::IsOpen())
		SetDraw(hv_WindowHandle, "margin");
		SetColor(hv_WindowHandle, "red");
	//if (HDevWindowStack::IsOpen())
		DispObj(ho_BadObjects, hv_WindowHandle);
	CountObj(ho_BadObjects, &hv_Num);
	if (0 != (hv_Num>0))
	{
		(*hv_IsBad) = 1;
	}


	return;
}