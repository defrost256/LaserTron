#pragma once

#include "ofPolyline.h"
#include "ofPoint.h"
#include "ofxIldaPoly.h"

/**
 * Points calculated manually to represent numbers from 1-3 and the letters G, O
 * Used for displaying them with a laser projector
 */
class CountdownPoints
{
public:
    CountdownPoints();
    ~CountdownPoints();

    ofxIlda::Poly pFont_2_0;
    ofxIlda::Poly pFont_2_1;
    ofxIlda::Poly pFont_3_0;
    ofxIlda::Poly pFont_3_1;
    ofxIlda::Poly pFont_1_0;
    ofxIlda::Poly pFont_1_1;
    ofxIlda::Poly pFont_G_0;
    ofxIlda::Poly pFont_G_1;
    ofxIlda::Poly pFont_O_0;
    ofxIlda::Poly pFont_O_1;
    

private:

    ofPoint font_2_0[12] = {
	ofPoint(0.4584198, 0.4954834),
	ofPoint(0.4677277, 0.4733276),
	ofPoint(0.4927521, 0.4625854),
	ofPoint(0.5163574, 0.4675903),
	ofPoint(0.5313721, 0.4840088),
	ofPoint(0.5342407, 0.4954834),
	ofPoint(0.5156403, 0.4954834),
	ofPoint(0.5084991, 0.4811401),
	ofPoint(0.4920349, 0.4811401),
	ofPoint(0.4798737, 0.4876099),
	ofPoint(0.4777374, 0.4962158),
	ofPoint(0.4584198, 0.4954834)
    };

    ofPoint font_2_1[7] = {
	ofPoint(0.4605713, 0.5341187),
	ofPoint(0.4906158, 0.5004883),
	ofPoint(0.4970551, 0.5012207),
	ofPoint(0.4977722, 0.5133667),
	ofPoint(0.5335388, 0.5148315),
	ofPoint(0.534256, 0.5341187),
	ofPoint(0.4605713, 0.5341187)
    };

    ofPoint font_3_0[8] = {
	ofPoint(0.4648438, 0.4641724),
	ofPoint(0.4634094, 0.4820557),
	ofPoint(0.4941711, 0.4820557),
	ofPoint(0.4934692, 0.499939),
	ofPoint(0.5063171, 0.5006714),
	ofPoint(0.5356445, 0.4735107),
	ofPoint(0.536377, 0.4641724),
	ofPoint(0.4648438, 0.4641724)
    };

    ofPoint font_3_1[11] = {
	ofPoint(0.5191956, 0.5020142),
	ofPoint(0.515625, 0.5112915),
	ofPoint(0.4956055, 0.5184326),
	ofPoint(0.4741516, 0.5170288),
	ofPoint(0.4627075, 0.5184326),
	ofPoint(0.4612732, 0.5341797),
	ofPoint(0.494873, 0.5370483),
	ofPoint(0.5177917, 0.531311),
	ofPoint(0.5299377, 0.5213013),
	ofPoint(0.5371094, 0.5005493),
	ofPoint(0.5191956, 0.5020142)
    };

    ofPoint font_1_0[5] = {
	ofPoint(0.5179749, 0.4733887),
	ofPoint(0.5179749, 0.5427246),
	ofPoint(0.4993668, 0.543457),
	ofPoint(0.5015182, 0.4733887),
	ofPoint(0.5179749, 0.4733887)
    };

    ofPoint font_1_1[4] = {
	ofPoint(0.4843521, 0.5048828),
	ofPoint(0.4936523, 0.4991455),
	ofPoint(0.4943695, 0.4712524),
	ofPoint(0.4843521, 0.5048828)
    };

    ofPoint font_G_0[13] = {
	ofPoint(0.4817896, 0.4611053),
	ofPoint(0.4488306, 0.4625244),
	ofPoint(0.4216699, 0.4875641),
	ofPoint(0.4238062, 0.5111694),
	ofPoint(0.4402856, 0.5290527),
	ofPoint(0.4824609, 0.5326385),
	ofPoint(0.4839258, 0.5161743),
	ofPoint(0.4516992, 0.5140228),
	ofPoint(0.4395532, 0.5032959),
	ofPoint(0.4438257, 0.4847107),
	ofPoint(0.4595728, 0.4775543),
	ofPoint(0.4831934, 0.4789886),
	ofPoint(0.4817896, 0.4611053)
    };

    ofPoint font_G_1[6] = {
	ofPoint(0.4767236, 0.4975739),
	ofPoint(0.4809961, 0.5090179),
	ofPoint(0.461709, 0.5090179),
	ofPoint(0.4538354, 0.5047302),
	ofPoint(0.4516992, 0.4968414),
	ofPoint(0.4767236, 0.4975739)
    };

    ofPoint font_O_0[9] = {
	ofPoint(0.5393896, 0.4637146),
	ofPoint(0.5143347, 0.4751587),
	ofPoint(0.5028906, 0.4994812),
	ofPoint(0.5136328, 0.5259399),
	ofPoint(0.5407935, 0.5366821),
	ofPoint(0.5665503, 0.5245056),
	ofPoint(0.5779944, 0.4944763),
	ofPoint(0.5658484, 0.4737244),
	ofPoint(0.5393896, 0.4637146)
    };

    ofPoint font_O_1[7] = {
	ofPoint(0.5401831, 0.4823303),
	ofPoint(0.5237341, 0.4923401),
	ofPoint(0.5237341, 0.509491),
	ofPoint(0.5380164, 0.5166626),
	ofPoint(0.5551978, 0.5102234),
	ofPoint(0.5580664, 0.4937439),
	ofPoint(0.5401831, 0.4823303)
    };
};

