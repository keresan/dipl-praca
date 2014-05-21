#include "scorefusioner.h"

ScoreFusioner::ScoreFusioner() {

}

ScoreFusioner::ScoreFusioner(QVector<float> &weights) {
	_weights = weights;
}

void ScoreFusioner::setWeightsAsComplement(QVector<float> weights, float complement) {
	if(weights.size() == 0) {
		throw std::runtime_error("fusion(): vector of weights is empty");
	}

	_weights.clear();
	for(int i=0; i < weights.size(); i++ ) {
		if(weights.at(i) > complement) {
			throw std::runtime_error("setWeightsAsComplement(): value > complement");
		}
		_weights.append(complement - weights.at(i));
		//qDebug() << weights.at(i) << "->" << _weights.at(i);
	}
}

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

float ScoreFusioner::sum(QVector<float> &src) {
	float sum = 0;

	for(int i=0; i < src.size(); i++) {
		sum += src.at(i);
	}
	return sum;
}

float ScoreFusioner::wSum(QVector<float> &src) {
	float sum = 0;

	for(int i=0; i < src.size(); i++) {
		sum += src.at(i) * _weights.at(i);
	}
	return sum;
}
