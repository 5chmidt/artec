#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS 1
#endif // _MSC_VER 

#include <artec/sdk/project/IProject.h>
#include <artec/sdk/project/EntryInfo.h>
#include <artec/sdk/project/ProjectSettings.h>
#include <artec/sdk/project/ProjectLoaderSettings.h>
#include <artec/sdk/project/ProjectSaverSettings.h>
#include <artec/sdk/algorithms/Algorithms.h>
#include <artec/sdk/algorithms/IAlgorithm.h>
#include <artec/sdk/base/IModel.h>
#include <artec/sdk/base/IScan.h>
#include <artec/sdk/base/IFrameMesh.h>
#include <artec/sdk/base/ICompositeContainer.h>
#include <artec/sdk/base/ICompositeMesh.h>
#include <artec/sdk/base/AlgorithmWorkset.h>
#include <artec/sdk/base/IProgressInfo.h>
#include <artec/sdk/base/IJobObserver.h>
#include <artec/sdk/base/IJob.h>
#include <artec/sdk/base/TRef.h>
#include <artec/sdk/base/IArray.h>

#include <iomanip>
#include <iostream>
#include <string>

#include <stdlib.h>


using namespace artec::sdk;

std::wstring stringToWString(const std::string& src)
{
    std::wstring result(src.length(), L' ');
    mbstowcs(&result[0], src.c_str(), src.length());
    return result;
}

int main(int argc, char** argv)
{
    if (argc < 2)
    {
        std::cout << "Need to pass in a file path.";
        return -1;
    }

    const std::wstring srcProjectPath = stringToWString(argv[1]);
    base::TRef<project::IProject> srcProject;
    base::TRef<base::IModel> srcModel;
    if (project::openProject(&srcProject, srcProjectPath.c_str()) != base::ErrorCode_OK)
    {
        std::cout << "Failed to open file: ";
        std::cout << argv[1];
    }

    return 0;
}
