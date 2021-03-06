#include "scorefusioner.h"

ScoreFusioner::ScoreFusioner() {}

/**
 * @brief Constructor. Implicit method of fusion is weighted sum
 * @param weights Weights of weighted sum
 */
ScoreFusioner::ScoreFusioner(QVector<float> &weights) {
	_weights = weights;
}

/**
 * @brief Constructor. Implicit method of fusion is weighted sum. Set weights as complement.
 * @param weights Weights
 * @param complement Complement
 */
void ScoreFusioner::setWeightsAsComplement(QVector<float> weights, float complement) {
	if(weights.size() == 0) {
		throw std::runtime_error("fusion(): vector of weights is empty");
	}

	_weights.clear();
	for(int i=0; i < weights.size(); i++ ) {
		if(weights.at(i) > complement) {
			throw std::runtime_error("setWeightsAsComplement(): value > complement");
		}
		if(weights.at(i) >= complement) {
			_weights.append(0);
		} else {
			_weights.append(complement - weights.at(i));
		}
		//qDebug() << weights.at(i) << "->" << _weights.at(i);
	}
}

/**
 * @brief Fusion Vector of score.
 * @param src Vector of score.
 * @param fusionMethod Method of fusion
 * @return Fusion score
 */
float ScoreFusioner::fusion(QVector<float> &src, ScoreFusioner::FusionMethod fusionMethod) {
	if(fusionMethod == ScoreFusioner::WeightedSum && _weights.size() == 0) {
		throw std::runtime_error("fusion(): vector of weights not entered");
	}

	switch(fusionMethod) {
		case ScoreFusioner::Sum:
			return sum(src);
			break;
		case ScoreFusioner::WeightedSum:
			return wSum(src);
			break;
		default:
			assert(false);
	}

}
/**
 * @brief Sum method
 * @param src Vector of score to fusion
 * @return Fusion of score
 */
float ScoreFusioner::sum(QVector<float> &src) {
	float sum = 0;

	for(int i=0; i < src.size(); i++) {
		sum += src.at(i);
	}
	return sum;
}

/**
 * @brief Weighted sum method.
 * @param src Vector of score to fusion
 * @return Fusion of score
 */
float ScoreFusioner::wSum(QVector<float> &src) {
	float sum = 0;

	for(int i=0; i < src.size(); i++) {
		sum += src.at(i) * _weights.at(i);
	}
	return sum;
}
