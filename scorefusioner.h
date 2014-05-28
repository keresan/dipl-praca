#ifndef SCOREFUSIONER_H
#define SCOREFUSIONER_H

#include "common.h"

/**
 * @brief Class for score fusion.
 */
class ScoreFusioner
{
public:
	ScoreFusioner();
	ScoreFusioner(QVector<float> &weights);

	/**
	 * @brief Method of score fusion.
	 */
	enum FusionMethod {
		Sum,
		WeightedSum,
	};

	void setWeightsAsComplement(QVector<float> weights,float complement = 0.4);

	float fusion(QVector<float> &src, ScoreFusioner::FusionMethod fusionMethod);


	float sum(QVector<float> &src);
	float wSum(QVector<float> &src);


private:
	/**
	 * @brief Weight coeficients
	 */
	QVector<float> _weights;
};

#endif // SCOREFUSIONER_H
