// (C) Copyright 2002-2012 by Autodesk, Inc. 
//
// Permission to use, copy, modify, and distribute this software in
// object code form for any purpose and without fee is hereby granted, 
// provided that the above copyright notice appears in all copies and 
// that both that copyright notice and the limited warranty and
// restricted rights notice below appear in all supporting 
// documentation.
//
// AUTODESK PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// AUTODESK SPECIFICALLY DISCLAIMS ANY IMPLIED WARRANTY OF
// MERCHANTABILITY OR FITNESS FOR A PARTICULAR USE.  AUTODESK, INC. 
// DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//
// Use, duplication, or disclosure by the U.S. Government is subject to 
// restrictions set forth in FAR 52.227-19 (Commercial Computer
// Software - Restricted Rights) and DFAR 252.227-7013(c)(1)(ii)
// (Rights in Technical Data and Computer Software), as applicable.
//

//-----------------------------------------------------------------------------
//----- acrxEntryPoint.cpp
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "resource.h"
#include "acdb.h"

#include "rxregsvc.h"
#include "dbapserv.h"
#include "aced.h"
#include "acdocman.h"
#include "dbhandle.h"
#include "acaplmgr.h"
#include "dbents.h"
#include "dbmain.h"

//-----------------------------------------------------------------------------
#define szRDS _RXST("rpk")

//-----------------------------------------------------------------------------
//----- ObjectARX EntryPoint
class CAeccExplorerApp : public AcRxArxApp {

public:
	CAeccExplorerApp () : AcRxArxApp () {}

	virtual AcRx::AppRetCode On_kInitAppMsg (void *pkt) {
		// TODO: Load dependencies here

		// You *must* call On_kInitAppMsg here
		AcRx::AppRetCode retCode =AcRxArxApp::On_kInitAppMsg (pkt) ;
		
		// TODO: Add your initialization code here

		return (retCode) ;
	}

	virtual AcRx::AppRetCode On_kUnloadAppMsg (void *pkt) {
		// TODO: Add your code here

		// You *must* call On_kUnloadAppMsg here
		AcRx::AppRetCode retCode =AcRxArxApp::On_kUnloadAppMsg (pkt) ;

		// TODO: Unload dependencies here

		return (retCode) ;
	}

	virtual void RegisterServerComponents () {
	}
	
	// The ACED_ARXCOMMAND_ENTRY_AUTO macro can be applied to any static member 
	// function of the CAeccExplorerApp class.
	// The function should take no arguments and return nothing.
	//
	// NOTE: ACED_ARXCOMMAND_ENTRY_AUTO has overloads where you can provide resourceid and
	// have arguments to define context and command mechanism.
	
	// ACED_ARXCOMMAND_ENTRY_AUTO(classname, group, globCmd, locCmd, cmdFlags, UIContext)
	// ACED_ARXCOMMAND_ENTRYBYID_AUTO(classname, group, globCmd, locCmdId, cmdFlags, UIContext)
	// only differs that it creates a localized name using a string in the resource file
	//   locCmdId - resource ID for localized command

	// Modal Command with localized name
	// ACED_ARXCOMMAND_ENTRY_AUTO(CAeccExplorerApp, rpkMyGroup, MyCommand, MyCommandLocal, ACRX_CMD_MODAL)
	static void rpkMyGroupMyCommand () {
        CAcModuleResourceOverride * myResources = new CAcModuleResourceOverride;

		AcDbDatabase * pDb = acdbHostApplicationServices()->workingDatabase();
		AcDbBlockTable * pBlkTbl;
		Acad::ErrorStatus es = pDb->getBlockTable(pBlkTbl);
		if (Acad::eOk == es)
		{

			AcDbObjectId blkId;
			es = pBlkTbl->getAt(_T("*MODEL_SPACE"), blkId);
			if (Acad::eOk == es)
			{
				AcDbBlockTableRecord * pRec;
				es = acdbOpenAcDbObject((AcDbObject *&)pRec, blkId, AcDb::kForRead);
				if (Acad::eOk == es)
				{
					AcDbBlockTableRecordIterator * pIter;
					es = pRec->newIterator(pIter);
					for (pIter->start(); !pIter->done(); pIter->step())
					{
						AcDbEntity * pEnt;
						es = pIter->getEntity(pEnt);
						if (Acad::eOk == es)
						{
							acutPrintf(_T("%s"), pEnt->layer());
							pEnt->close();
						}

					}
					pRec->close();
					delete pIter;
				}
			}
			pBlkTbl->close();
			
		}


		//AcDbBlockTableIterator * pIter;
		//es = pBlkTbl->newIterator(pIter);
		//for(pIter->start(); !pIter->done(); pIter->step())
		//{ 
		//	AcDbBlockTableRecord * pRcd;
		//	pIter->getRecord(pRcd);
		//	const TCHAR * blkName;
		//	pRcd->getName(blkName);
		//	acutPrintf(_T("%s"), blkName);
		//	pRcd->close();
		//}
		//delete pIter;
		//pBlkTbl->close();
		//
  //      ads_name ss;
		//resbuf * rb = ads_buildlist(RTDXF0, _T("AECC_*"), 0);

		//if (RTNORM == acedSSGet(_T("X"), NULL, NULL, rb, ss))
		//{
		//	Adesk::Int32 nCount;
		//	acedSSLength(ss, &nCount);
		//	for (Adesk::Int32 i = 0; i < nCount; i++)
		//	{
		//		ads_name entName;
		//		if (acedSSName(ss, i, entName) == RTNORM)
		//		{
		//			AcDbObjectId eId;
		//			acdbGetObjectId(eId, entName);
		//			AcDbEntity *pEntity;
		//			es = acdbOpenAcDbEntity(pEntity, eId, AcDb::kForRead);
		//			if (es == Acad::eOk)
		//			{
		//				AcDbObjectId ownerId = pEntity->blockId();
		//				AcDbBlockTableRecord * pObj;
		//				es = acdbOpenAcDbObject((AcDbObject*&) pObj, ownerId, AcDb::kForRead);
		//				if (es == Acad::eOk)
		//				{
		//					pObj->close();
		//				}



		//				pEntity->close();
		//			}
		//		}
		//	}
		//	acedSSFree(ss);
		//}
		//acutRelRb(rb);
	}

	// Modal Command with pickfirst selection
	// ACED_ARXCOMMAND_ENTRY_AUTO(CAeccExplorerApp, rpkMyGroup, MyPickFirst, MyPickFirstLocal, ACRX_CMD_MODAL | ACRX_CMD_USEPICKSET)
	static void rpkMyGroupMyPickFirst () {
		ads_name result ;
		int iRet =acedSSGet (ACRX_T("_I"), NULL, NULL, NULL, result) ;
		if ( iRet == RTNORM )
		{
			// There are selected entities
			// Put your command using pickfirst set code here
		}
		else
		{
			// There are no selected entities
			// Put your command code here
		}
	}

	// Application Session Command with localized name
	// ACED_ARXCOMMAND_ENTRY_AUTO(CAeccExplorerApp, rpkMyGroup, MySessionCmd, MySessionCmdLocal, ACRX_CMD_MODAL | ACRX_CMD_SESSION)
	static void rpkMyGroupMySessionCmd () {
		// Put your command code here
	}

	// The ACED_ADSFUNCTION_ENTRY_AUTO / ACED_ADSCOMMAND_ENTRY_AUTO macros can be applied to any static member 
	// function of the CAeccExplorerApp class.
	// The function may or may not take arguments and have to return RTNORM, RTERROR, RTCAN, RTFAIL, RTREJ to AutoCAD, but use
	// acedRetNil, acedRetT, acedRetVoid, acedRetInt, acedRetReal, acedRetStr, acedRetPoint, acedRetName, acedRetList, acedRetVal to return
	// a value to the Lisp interpreter.
	//
	// NOTE: ACED_ADSFUNCTION_ENTRY_AUTO / ACED_ADSCOMMAND_ENTRY_AUTO has overloads where you can provide resourceid.
	
	//- ACED_ADSFUNCTION_ENTRY_AUTO(classname, name, regFunc) - this example
	//- ACED_ADSSYMBOL_ENTRYBYID_AUTO(classname, name, nameId, regFunc) - only differs that it creates a localized name using a string in the resource file
	//- ACED_ADSCOMMAND_ENTRY_AUTO(classname, name, regFunc) - a Lisp command (prefix C:)
	//- ACED_ADSCOMMAND_ENTRYBYID_AUTO(classname, name, nameId, regFunc) - only differs that it creates a localized name using a string in the resource file

	// Lisp Function is similar to ARX Command but it creates a lisp 
	// callable function. Many return types are supported not just string
	// or integer.
	// ACED_ADSFUNCTION_ENTRY_AUTO(CAeccExplorerApp, MyLispFunction, false)
	static int ads_MyLispFunction () {
		//struct resbuf *args =acedGetArgs () ;
		
		// Put your command code here

		//acutRelRb (args) ;
		
		// Return a value to the AutoCAD Lisp Interpreter
		// acedRetNil, acedRetT, acedRetVoid, acedRetInt, acedRetReal, acedRetStr, acedRetPoint, acedRetName, acedRetList, acedRetVal

		return (RTNORM) ;
	}
	
} ;

//-----------------------------------------------------------------------------
IMPLEMENT_ARX_ENTRYPOINT(CAeccExplorerApp)

ACED_ARXCOMMAND_ENTRY_AUTO(CAeccExplorerApp, rpkMyGroup, MyCommand, MyCommandLocal, ACRX_CMD_MODAL, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CAeccExplorerApp, rpkMyGroup, MyPickFirst, MyPickFirstLocal, ACRX_CMD_MODAL | ACRX_CMD_USEPICKSET, NULL)
// ACED_ARXCOMMAND_ENTRY_AUTO(CAeccExplorerApp, rpkMyGroup, MySessionCmd, MySessionCmdLocal, ACRX_CMD_MODAL | ACRX_CMD_SESSION, NULL)
// ACED_ADSSYMBOL_ENTRY_AUTO(CAeccExplorerApp, MyLispFunction, false)

