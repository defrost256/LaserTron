#include "CountdownPoints.h"

CountdownPoints::CountdownPoints()
{
    vector<ofPoint> tmp;
    for (int i = 0; i < 12; i++)
	tmp.push_back(font_2_0[i]);
    pFont_2_0 = ofxIlda::Poly(tmp);
    tmp.clear();

    for (int i = 0; i < 7; i++)
	tmp.push_back(font_2_1[i]);
    pFont_2_1 = ofxIlda::Poly(tmp);
    tmp.clear();

    for (int i = 0; i < 8; i++)
	tmp.push_back(font_3_0[i]);
    pFont_3_0 = ofxIlda::Poly(tmp);
    tmp.clear();

    for (int i = 0; i < 11; i++)
	tmp.push_back(font_3_1[i]);
    pFont_3_1 = ofxIlda::Poly(tmp);
    tmp.clear();

    for (int i = 0; i < 5; i++)
	tmp.push_back(font_1_0[i]);
    pFont_1_0 = ofxIlda::Poly(tmp);
    tmp.clear();

    for (int i = 0; i < 4; i++)
	tmp.push_back(font_1_1[i]);
    pFont_1_1 = ofxIlda::Poly(tmp);
    tmp.clear();

    for (int i = 0; i < 13; i++)
	tmp.push_back(font_G_0[i]);
    pFont_G_0 = ofxIlda::Poly(tmp);
    tmp.clear();

    for (int i = 0; i < 6; i++)
	tmp.push_back(font_G_1[i]);
    pFont_G_1 = ofxIlda::Poly(tmp);
    tmp.clear();

    for (int i = 0; i < 9; i++)
	tmp.push_back(font_O_0[i]);
    pFont_O_0 = ofxIlda::Poly(tmp);
    tmp.clear();

    for (int i = 0; i < 7; i++)
	tmp.push_back(font_O_1[i]);
    pFont_O_1 = ofxIlda::Poly(tmp);
}


CountdownPoints::~CountdownPoints()
{
}
