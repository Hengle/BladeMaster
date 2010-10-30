#include "GraphicsPrivate.h"
#include "ShaderMgr.h"
#include "Shlwapi.h"

SINGLETON_DEFINE(ShaderMgr);

ShaderMgr::ShaderMgr()
{

}

ShaderMgr::~ShaderMgr()
{

}

/*
 *	when meeting fx, search for xml for macro definition
 *  
 */
void ShaderMgr::LoadShaders()
{
    const wchar_t* shaderPath = L"..\\Engine\\Shader\\*.*";
    const wchar_t* xmlEx = L".xml";
    const wchar_t* fxEx = L".fx";

    /*
     *	construct name list
     */
    std::vector<std::wstring> fileNames;

    WIN32_FIND_DATA stData; 
    HANDLE hFind = FindFirstFile(shaderPath, &stData); 
    if (hFind  == INVALID_HANDLE_VALUE)  
        return;
    
    do 
    {        
        if(StrCmpW(stData.cFileName, L".")==0 || StrCmpW(stData.cFileName, L"..")==0)
        {
            continue;
        }

        fileNames.push_back(stData.cFileName);

    } while (FindNextFile(hFind, &stData));

    /*
     *	parse name list, construct description list
     */
    for(s32 i=0; i<fileNames.size(); i++)    
    {
        const std::wstring& fileName = fileNames[i];
        size_t pos = fileName.find(fxEx);
        if(pos==std::wstring.npos)
        {
            continue;
        }

        mShaderDescs.push_back(ShaderDesc());
        ShaderDesc& shaderDesc = mShaderDescs[mShaderDescs.size()-1];

        std::wstring name = fileName.substr(0, fileName.size()-3);
        name +=xmlEx;
        std::vector<std::wstring>::iterator result = std::find(fileNames.begin(), fileNames.end(), name);
                
        if(result!=fileNames.end())
        {
            //parse xml for description
            std::wstring xmlNameW = shaderPath;
            xmlNameW+=name;
            std::string xmlName;
            WStrToStr(xmlNameW, xmlName);

            TiXmlDocument xmlDoc;
            bool result = xmlDoc.LoadFile(xmlName);            
            assert(result);

            TiXmlElement *RootElement = xmlDoc.RootElement();        

            for(TiXmlElement* ele_it = RootElement->FirstChildElement(); ele_it; ele_it = (TiXmlElement*)ele_it->NextSibling())
            {
                for(TiXmlAttribute* attr_it = ele_it->FirstAttribute(); attr_it; attr_it = attr_it->Next())
                {
                    const char* name = attr_it->Name();

                    if(strcmp("name", name)==0)
                    {
                        shaderDesc.definitions.push_back(attr_it->Value());
                    }
                }     
            }
        }

        WStrToStr(fileName,shaderDesc.name);
    }
}