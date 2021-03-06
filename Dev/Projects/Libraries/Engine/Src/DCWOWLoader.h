#pragma once

class DCModel;
class DCTexture;
class DCMaterial;
class CreatureDisplayData;
class CreatureModelData;
class DCFilePath;

class ENGINE_DLL DCWOWLoader
{
    SINGLETON_DECLARE(DCWOWLoader);

public:
	DCModel*							LoadModel(DCFilePath& _path, DCAnimationManager*& animMgr);
	DCModel*							LoadModel(u32 creatureID);
	DCTexture*							LoadTexture(const std::string& _filename);

	void Init();

    //attribtues
private:
	CreatureDisplayData*				mCreatureDisplay;
	CreatureModelData*					mCreatureModel;
	std::vector<MPQArchive*>			mArchives;
};
