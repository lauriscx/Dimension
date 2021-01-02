#pragma once
#include "GraphicObject.h"
#include "Batch.h"
#include "Texture.h"

void Batch::AddBatch(Batch batch) {
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
void Batch::AddToBatch(GraphicObject GrahicObjectData) {
		int prevIndexSize = Indices.size();
		int NewMaxIndeci = 0;
		for (int i = 0; i < GrahicObjectData.Indices.size(); i++) {
			int newIndices = IndexNumber + GrahicObjectData.Indices[i];
			this->Indices.push_back(newIndices);
			if (newIndices > NewMaxIndeci) {
				NewMaxIndeci = newIndices;
			}
		}

		bool addTexture = true;
		int textID = 0;
		for (Texture tex : TexturesBatch) {
			if (strcmp(tex.GetTextureName().c_str(), GrahicObjectData.GetMaterial()->GetTexture("diffuseMap").GetTextureName().c_str()) == 0) {
				addTexture = false;
				for (int i = IndexNumber; i < NewMaxIndeci + 1; i++) {
					this->TexturesIndex.push_back(textID);
				}
				break;
			}
			textID++;
		}
		if (addTexture) {
			TexturesBatch.push_back(GrahicObjectData.GetMaterial()->GetTexture("diffuseMap"));

			for (int i = IndexNumber; i < NewMaxIndeci + 1; i++) {
				this->TexturesIndex.push_back(TexturesBatch.size() - 1);
			}
		}
		/*else {
			//get texture ID from this list TexturesIndex.
			for (int i = IndexNumber; i < NewMaxIndeci + 1; i++) {
				this->TexturesIndex.push_back(-1);
			}
		}*/

		for (int i = IndexNumber; i < NewMaxIndeci + 1; i++) {
			this->VertextObjectIndex.push_back(ObjectCount);
		}

		IndexNumber = NewMaxIndeci + 1;
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
		this->ObjectColor.push_back(GrahicObjectData.GetMaterial()->color);
		ObjectCount++;
	}
void Batch::ResetBatchUniforms() {
		transformations.clear();
		ObjectColor.clear();
	}
void Batch::UpdateBatchUniforms(GraphicObject GrahicObjectData) {
		this->transformations.push_back(GrahicObjectData.GetTransformation());
		this->ObjectColor.push_back(GrahicObjectData.GetMaterial()->color);
}

void Batch::ClearBatch() {
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

		VertextObjectIndex.clear();
		transformations.clear();
		ObjectColor.clear();
		//TexturesBatch.clear();

		ObjectCount = 0;
		IndexNumber = 0;
}