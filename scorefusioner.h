#ifndef SCOREFUSIONER_H
#define SCOREFUSIONER_H

#include "common.h"

class ScoreFusioner
{
public:
	ScoreFusioner();
	ScoreFusioner(QVector<float> &weights);

	enum FusionMethod {
		Sum,
		WeightedSum,
	};

	void setWeightsAsComplement(QVector<float> weights,float complement = 0.5);

	float fusion(QVector<float> &src, ScoreFusioner::FusionMethod fusionMethod);


	float sum(QVector<float> &src);
	float wSum(QVector<float> &src);


private:
	QVector<float> _weights;
};

#endif // SCOREFUSIONER_H
