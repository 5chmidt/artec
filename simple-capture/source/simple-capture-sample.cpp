/********************************************************************
*
*	Project		Artec 3D Scanning SDK Samples
*
*	Purpose:	Simple capture sample
*
*	Copyright:	Artec Group
*
********************************************************************/

#undef NDEBUG

#include <string>
#include <iostream>

#include <artec/sdk/capturing/IScanner.h>
#include <artec/sdk/capturing/IArrayScannerId.h>
#include <artec/sdk/capturing/IFrameProcessor.h>
#include <artec/sdk/capturing/IFrame.h>
#include <artec/sdk/base/BaseSdkDefines.h>
#include <artec/sdk/base/Log.h>
#include <artec/sdk/base/io/ObjIO.h>
#include <artec/sdk/base/IFrameMesh.h>
#include <artec/sdk/base/TArrayRef.h>

namespace asdk {
    using namespace artec::sdk::base;
    using namespace artec::sdk::capturing;
};
using asdk::TRef;
using asdk::TArrayRef;

int main( int argc, char **argv )
{
    // The log verbosity level is set here. It is set to the most
    // verbose value - Trace. If you have any problems working with 
    // our examples, please do not hesitate to send us this extensive 
    // information along with your questions. However, if you feel 
    // comfortable with these Artec Scanning SDK code examples,
    // we suggest you to set this level to asdk::VerboseLevel_Info.
    asdk::setOutputLevel( asdk::VerboseLevel_Trace );

	asdk::ErrorCode ec = asdk::ErrorCode_OK;

    TRef<asdk::IArrayScannerId> scannersList;

	std::wcout << L"Enumerating scanners... ";
	ec = asdk::enumerateScanners( &scannersList );
	if( ec != asdk::ErrorCode_OK )
	{
		std::wcout << L"failed" << std::endl;
		return 1;
	}
	std::wcout << L"done" << std::endl;

	int scanner_count = scannersList->getSize();
    if( scanner_count == 0 )
    {
        std::wcout << L"No scanners found" << std::endl;
        return 3;
    }

    const asdk::ScannerId* idArray = scannersList->getPointer();

    const asdk::ScannerId& defaultScanner = idArray[0]; // just take the first available scanner

    std::wcout 
        << L"Connecting to " << asdk::getScannerTypeName( defaultScanner.type ) 
        << L" scanner " << defaultScanner.serial << L"... "
    ;

    TRef<asdk::IScanner> scanner;
    ec = asdk::createScanner( &scanner, &defaultScanner );

    if( ec != asdk::ErrorCode_OK )
    {
        std::wcout << L"failed" << std::endl;
        return 2;
    }
    std::wcout << L"done" << std::endl;

	std::wcout << L"Capturing frame... ";

    TRef<asdk::IFrame> frame;
    TRef<asdk::IFrameMesh> mesh;

    TRef<asdk::IFrameProcessor> processor;
	ec = scanner->createFrameProcessor( &processor );
	if( ec == asdk::ErrorCode_OK )
	{
        frame = NULL;
		ec = scanner->capture( &frame, true ); // with texture
		if( ec == asdk::ErrorCode_OK )
		{
            mesh = NULL;
			ec = processor->reconstructAndTexturizeMesh( &mesh, frame );
			if( ec == asdk::ErrorCode_OK )
			{
				std::wcout << L"done" << std::endl;
				// save the mesh
				ec = asdk::io::Obj::save( L"frame.obj", mesh ); // save in text format

				// working with normals
				// 1. generate normals
				mesh->calculate( asdk::CM_Normals );

				// 2. get normals array using helper class
				asdk::TArrayPoint3F pointsNormals  = mesh->getPointsNormals();

				// 3. get number of normals
				int normalCount = pointsNormals.size();
                ASDK_UNUSED(normalCount);

				// 4. use normal
				asdk::Point3F point = pointsNormals[0];
                ASDK_UNUSED(point);

				std::wcout << L"Captured mesh saved to disk" << std::endl;
			}
			else
			{
				std::wcout << L"failed" << std::endl;
			}
		}
	}

    scanner = NULL;
	std::wcout << L"Scanner released" << std::endl;

    return 0;
}
