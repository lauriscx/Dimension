#pragma once
#include <glm/glm.hpp>
#include <vector>
#include "GraphicObject.h"

class Batch {
public:
	void AddBatch(Batch batch) {
		this->Indices.insert(std::end(this->Indices), std::begin(batch.Indices), std::end(batch.Indices));

		this->UniformsIndex.insert(std::end(this->UniformsIndex), std::begin(batch.UniformsIndex), std::end(batch.UniformsIndex));
		this->TexturesIndex.insert(std::end(this->TexturesIndex), std::begin(batch.TexturesIndex), std::end(batch.TexturesIndex));

		this->Positions.insert(std::end(this->Positions), std::begin(batch.Positions), std::end(batch.Positions));
		this->Normals.insert(std::end(this->Normals), std::begin(batch.Normals), std::end(batch.Normals));
		this->Tangents.insert(std::end(this->Tangents), std::begin(batch.Tangents), std::end(batch.Tangents));
		this->BiTangents.insert(std::end(this->BiTangents), std::begin(batch.BiTangents), std::end(batch.BiTangents));

		this->Colors.insert(std::end(this->Colors), std::begin(batch.Colors), std::end(batch.Colors));

		this->TexturesCoordinates.insert(std::end(this->TexturesCoordinates), std::begin(batch.TexturesCoordinates), std::end(batch.TexturesCoordinates));

		this->BonesIndex.insert(std::end(this->BonesIndex), std::begin(batch.BonesIndex), std::end(batch.BonesIndex));
		this->BonesWeights.insert(std::end(this->BonesWeights), std::begin(batch.BonesWeights), std::end(batch.BonesWeights));
	}
	void AddToBatch(GraphicObject GrahicObjectData) {
		this->Indices.insert(std::end(this->Indices), std::begin(GrahicObjectData.Indices), std::end(GrahicObjectData.Indices));

		this->UniformsIndex.insert(std::end(this->UniformsIndex), std::begin(GrahicObjectData.UniformsIndex), std::end(GrahicObjectData.UniformsIndex));
		this->TexturesIndex.insert(std::end(this->TexturesIndex), std::begin(GrahicObjectData.TexturesIndex), std::end(GrahicObjectData.TexturesIndex));

		this->Positions.insert(std::end(this->Positions), std::begin(GrahicObjectData.Positions), std::end(GrahicObjectData.Positions));
		this->Normals.insert(std::end(this->Normals), std::begin(GrahicObjectData.Normals), std::end(GrahicObjectData.Normals));
		this->Tangents.insert(std::end(this->Tangents), std::begin(GrahicObjectData.Tangents), std::end(GrahicObjectData.Tangents));
		this->BiTangents.insert(std::end(this->BiTangents), std::begin(GrahicObjectData.BiTangents), std::end(GrahicObjectData.BiTangents));

		this->Colors.insert(std::end(this->Colors), std::begin(GrahicObjectData.Colors), std::end(GrahicObjectData.Colors));

		this->TexturesCoordinates.insert(std::end(this->TexturesCoordinates), std::begin(GrahicObjectData.TexturesCoordinates), std::end(GrahicObjectData.TexturesCoordinates));

		this->BonesIndex.insert(std::end(this->BonesIndex), std::begin(GrahicObjectData.BonesIndex), std::end(GrahicObjectData.BonesIndex));
		this->BonesWeights.insert(std::end(this->BonesWeights), std::begin(GrahicObjectData.BonesWeights), std::end(GrahicObjectData.BonesWeights));
		
		this->transformations.push_back(GrahicObjectData.GetTransformation());
	}

	void ClearBatch() {
		Indices.clear();
		UniformsIndex.clear();
		TexturesIndex.clear();

		Positions.clear();
		Normals.clear();
		Tangents.clear();
		BiTangents.clear();

		Colors.clear();

		TexturesCoordinates.clear();

		BonesIndex.clear();
		BonesWeights.clear();
	}

	std::vector<int> Indices;
	std::vector<int> UniformsIndex;
	std::vector<int> TexturesIndex;

	std::vector<float> Positions;
	std::vector<float> Normals;
	std::vector<float> Tangents;
	std::vector<float> BiTangents;

	std::vector<float> Colors;

	std::vector<float> TexturesCoordinates;

	//For animations 3D
	std::vector<int> BonesIndex;
	std::vector<float> BonesWeights;

	std::vector<glm::mat4> transformations;
};