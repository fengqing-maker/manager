#include "stdafx.h"
#include "EP61850Client.h"
#include "AbstractFileManager.h"

#include "XMLDOMFile.h"
#include "UtilityTool.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

AbstractFileManager::AbstractFileManager(void)
{
	m_xmlFileOpened = FALSE;
	m_xmlFileName.Empty();
	m_xmlFilePath.Empty();
	m_xmlModelMap.clear();

	m_xmlModelParser.reset();
	m_xmlFileThread.reset();
}
AbstractFileManager::~AbstractFileManager(void)
{
}

BOOL AbstractFileManager::IsFileOpened()
{
	return m_xmlFileOpened;
}
BOOL AbstractFileManager::IsFileModified()
{
	Weak_XMLModel weak_model = CurrentFileModel();
	Shared_XMLModel shared_model = weak_model.lock();
	if (shared_model == 0)
		return FALSE;
	CXMLDOMFilePtr domFile = shared_model->GetModelDOMFile();
	if (!domFile || !domFile->IsFileOpened())
		return FALSE;
	return domFile->GetXMLFileModified();
}

BOOL AbstractFileManager::NewFile(XmlFileCreateCtrl fileCreateCtrl)
{
	if (IsFileOpened())
	{
		CString err;
		err.Format(TRANS_T("�ļ��½���������ʧ�ܣ���ǰ���ļ�δ�ر�."));
		OutputErr(err);
		return FALSE;
	}
	m_xmlFileThread.reset();
	if (m_xmlFileThread == 0)
	{
		m_xmlFileThread = GetFileThread();
		if(m_xmlFileThread == 0)
		{
			CString err;
			err.Format(TRANS_T("�ļ��½���������ʧ��,δ�������߳�."));
			OutputErr(err);
			return FALSE;
		}
	}
	if (!m_xmlFileThread->isRunning())
	{
		m_xmlFileThread->create();
		if (!m_xmlFileThread->isRunning())
		{
			CString err;
			err.Format(TRANS_T("�ļ��½���������ʧ��,���߳�����ʧ��."));
			OutputErr(err);
			return FALSE;
		}
	}
	m_xmlFileName = CUtilityTool::GetObj()->GetFileNameByPath(fileCreateCtrl.filePath);
	m_xmlFilePath = fileCreateCtrl.filePath;
	m_xmlFileCreateCtrl = fileCreateCtrl;
	m_xmlFileThread->addObserver(m_msgHandlerList);
	m_xmlFileThread->startFileNew();
	return TRUE;
}
BOOL AbstractFileManager::OpenFile(CString filePath)
{
	if (IsFileOpened())
	{
		CString err;
		err.Format(TRANS_T("�ļ��򿪲�������ʧ�ܣ���ǰ���ļ�δ�ر�."));
		OutputErr(err);
		return FALSE;
	}
	m_xmlFileThread.reset();
	if (m_xmlFileThread == 0)
	{
		m_xmlFileThread = GetFileThread();
		if(m_xmlFileThread == 0)
		{
			CString err;
			err.Format(TRANS_T("�ļ��򿪲�������ʧ��,δ�������߳�."));
			OutputErr(err);
			return FALSE;
		}
	}
	if (!m_xmlFileThread->isRunning())
	{
		m_xmlFileThread->create();
		if (!m_xmlFileThread->isRunning())
		{
			CString err;
			err.Format(TRANS_T("�ļ��򿪲�������ʧ��,���߳�����ʧ��."));
			OutputErr(err);
			return FALSE;
		}
	}
	m_xmlFilePath = filePath;
	m_xmlFileName = CUtilityTool::GetObj()->GetFileNameByPath(filePath);
	m_xmlFileThread->addObserver(m_msgHandlerList);
	m_xmlFileThread->startFileOpen();
	return TRUE;
}
BOOL AbstractFileManager::CloseFile()
{
	if (!IsFileOpened())
	{
		CString err = TRANS_T("�ļ��ر�ʧ�ܣ���ǰδ�򿪡�");
		OutputErr(err);
		return FALSE;
	}
	if (IsFileModified())
	{
		CWnd* pMainFrm = theApp.GetMainWnd();
		HWND hMainFrm = pMainFrm->GetSafeHwnd();
		int choice = ::MessageBox(hMainFrm,TRANS_T("�ļ��Ѿ����޸ģ��Ƿ񱣴���ģ�\n����������������δ������Ľ��ᶪʧ��"),
			TRANS_T("�����ļ�?"),
			MB_YESNOCANCEL | MB_ICONQUESTION);
		switch (choice)
		{
		case IDYES:
			if(!SaveFile())
			{
				CString err = TRANS_T("�ļ��ر�ʧ�ܣ������ļ�ʧ�ܡ�");
				OutputErr(err);
				return FALSE;	/* �����ļ�ʧ�ܣ�ȡ���ر��ļ� */
			}
			break;
		case IDNO:
			break;
		case IDCANCEL:
			return FALSE;
		default:
			break;
		}
		
	}
	m_xmlFileThread.reset();
	if (m_xmlFileThread == 0)
	{
		m_xmlFileThread = GetFileThread();
		if(m_xmlFileThread == 0)
		{
			CString err;
			err.Format(TRANS_T("�ļ��رղ�������ʧ��,δ�������߳�."));
			OutputErr(err);
			return FALSE;
		}
	}
	if (!m_xmlFileThread->isRunning())
	{
		m_xmlFileThread->create();
		if (!m_xmlFileThread->isRunning())
		{
			CString err;
			err.Format(TRANS_T("�ļ��رղ�������ʧ��,���߳�����ʧ��."));
			OutputErr(err);
			return FALSE;
		}
	}
	m_xmlFileThread->addObserver(m_msgHandlerList);
	m_xmlFileThread->startFileClose();
	return TRUE;
}
BOOL AbstractFileManager::SaveFile()
{
	if (!IsFileOpened())
	{
		CString err = TRANS_T("�ļ�����ʧ�ܣ���ǰδ���ļ���");
		OutputErr(err);
		return FALSE;
	}
	CString filePath = CurrentFilePath();
	Weak_XMLModel weak_model = CurrentFileModel();
	Shared_XMLModel shared_model = weak_model.lock();
	ASSERT(shared_model != 0);
	CXMLDOMFilePtr domFile = shared_model->GetModelDOMFile();
	DesBool desRet = domFile->Save();
	if (desRet.result)
	{
		CString msg;
		msg.Format(_T("�ļ�\"%s\"����ɹ�."),
			filePath);
		OutputMsg(msg);
		return TRUE;
	} 
	else
	{
		CString err;
		err.Format(_T("�ļ�\"%s\"����ʧ��.\nԭ��%s"),
			filePath,desRet.description);
		OutputErr(err);
		return FALSE;
	}
}
BOOL AbstractFileManager::SaveAsFile(CString newFilePath)
{
	if (!IsFileOpened())
	{
		CString err = TRANS_T("�ļ����Ϊʧ�ܣ���ǰδ���ļ���");
		OutputErr(err);
		return FALSE;
	}

	CString filePath = CurrentFilePath();
	Weak_XMLModel weak_model = CurrentFileModel();
	Shared_XMLModel shared_model = weak_model.lock();
	ASSERT(shared_model != 0);
	CXMLDOMFilePtr domFile = shared_model->GetModelDOMFile();
	DesBool desRet = domFile->Save(newFilePath);
	if (desRet.result)
	{
		Shared_XMLModelMapItor itor = m_xmlModelMap.find(m_xmlFilePath);
		Shared_XMLModel shared_model = itor->second;
		ASSERT(shared_model != 0);
		m_xmlModelMap.erase(itor);
		m_xmlFileName = CUtilityTool::GetObj()->GetFileNameByPath(newFilePath);
		m_xmlFilePath = newFilePath;
		m_xmlModelMap.insert(std::make_pair(m_xmlFilePath,shared_model));
		
		CString msg;
		msg.Format(_T("�ļ�\"%s\" ���Ϊ \"%s\"�ɹ�."),
			filePath,newFilePath);
		OutputMsg(msg);
		return TRUE;
	} 
	else
	{
		CString err;
		err.Format(_T("�ļ�\"%s\" ���Ϊ \"%s\"ʧ��.\nԭ��%s"),
			filePath,newFilePath,desRet.description);
		OutputErr(err);
		return FALSE;
	}
}

CString AbstractFileManager::CurrentFileName()
{
	if (IsFileOpened())
	{
		ASSERT(!m_xmlFileName.IsEmpty());
	}
	return m_xmlFileName;
}
CString AbstractFileManager::CurrentFilePath()
{
	if (IsFileOpened())
	{
		ASSERT(!m_xmlFilePath.IsEmpty());
	}
	return m_xmlFilePath;
}
Weak_XMLModel AbstractFileManager::CurrentFileModel()
{
	Weak_XMLModel weak_model;
	if (IsFileOpened())
	{
		Shared_XMLModelMapItor itor = m_xmlModelMap.find(m_xmlFilePath);
		ASSERT(itor != m_xmlModelMap.end());
		weak_model = itor->second;
	}
	return weak_model;
}
Weak_XMLElem AbstractFileManager::CurrentFileRootElem()
{
	Weak_XMLElem weak_root;
	if (IsFileOpened())
	{
		Shared_XMLModelMapItor itor = m_xmlModelMap.find(m_xmlFilePath);
		ASSERT(itor != m_xmlModelMap.end());
		Shared_XMLModel	xmlModel = itor->second;
		weak_root = xmlModel->GetModelRootElem();
	}
	return weak_root;
}

BOOL AbstractFileManager::doNewFile()
{
	BOOL result = FALSE;
	while (1)
	{
		/* �û�����·���ļ��в�����ʱ���д��� */
		CString dirPath = CUtilityTool::GetObj()->GetDirPathByFullPath(m_xmlFileCreateCtrl.filePath);
		if(!CUtilityTool::GetObj()->CreateDir(dirPath))
		{
			CString err;
			err.Format(TRANS_T("�ļ�\"%s\"����ʧ�ܡ�\nԭ���ļ���\"%s\"����ʧ�ܡ�"),
				dirPath);
			OutputErr(err);
			result = FALSE;
			break;
		}

		CXMLSOMFilePtr xmlSOMFile = NULL;
		DesBool desRet;
		if (!m_xmlFileCreateCtrl.schemaLocation.IsEmpty()
			&& !m_xmlFileCreateCtrl.rootNamespaceURI.IsEmpty())
		{
			xmlSOMFile.reset(new CXMLSOMFile());
			desRet = xmlSOMFile->Open
				(m_xmlFileCreateCtrl.schemaLocation,m_xmlFileCreateCtrl.rootNamespaceURI);
			if (desRet.result == FALSE)
			{
				CString err;
				err.Format(TRANS_T("�ļ�\"%s\"����ʧ�ܡ�\nԭ��Schema�ļ���ʧ�ܡ�"),
					m_xmlFileCreateCtrl.filePath);
				OutputErr(err);
				result = FALSE;
				break;
			}
		}

		CXMLDOMFilePtr xmlDOMFile(new CXMLDOMFile());
		desRet = xmlDOMFile->New(m_xmlFileCreateCtrl.filePath,
			m_xmlFileCreateCtrl.rootName,m_xmlFileCreateCtrl.rootNamespaceURI,xmlSOMFile);
		if (desRet.result == FALSE)
		{
			CString err;
			err.Format(TRANS_T("�ļ�\"%s\"����ʧ�ܡ�\nԭ��DOM�ļ�����ʧ�ܡ�"),
				m_xmlFileCreateCtrl.filePath);
			OutputErr(err);
			result = FALSE;
			break;
		}
		desRet = xmlDOMFile->Open(m_xmlFileCreateCtrl.filePath,xmlSOMFile);
		if(!desRet.result)
		{
			CString err;
			err.Format(TRANS_T("�ļ�\"%s\"����ʧ�ܡ�\nԭ��DOM�ļ�����ʧ�ܡ�"),
				m_xmlFileCreateCtrl.filePath);
			OutputErr(err);
			result = FALSE;
			break;
		}
		m_xmlModelParser = GetModelParser();
		if (m_xmlModelParser == 0)
		{
			CString err;
			err.Format(TRANS_T("�ļ�\"%s\"����ʧ�ܡ�\nԭ�򣺽���������ʧ�ܡ�"),
				m_xmlFileCreateCtrl.filePath);
			OutputErr(err);
			result = FALSE;
			break;
		}
		Shared_XMLModel	xmlModel = m_xmlModelParser->ParseXMLDOM(xmlDOMFile);
		m_xmlModelParser.reset();
		if (xmlModel == 0)
		{
			CString err;
			err.Format(TRANS_T("�ļ�\"%s\"����ʧ�ܡ�\nԭ��ģ�ͽ���ʧ�ܡ�"),
				m_xmlFileCreateCtrl.filePath);
			OutputErr(err);
			result = FALSE;	/* ����ʧ�� */
			break;
		}
		m_xmlModelMap.insert(std::make_pair(m_xmlFileCreateCtrl.filePath,xmlModel));
		result = TRUE;
		break;
	}
	m_xmlFileOpened = result;
	if (result != TRUE)
	{
		m_xmlFileName.Empty();
		m_xmlFilePath.Empty();
	}
	else
	{
		CString msg;
		msg.Format(TRANS_T("�ļ�\"%s\"�����ɹ���"),
			m_xmlFileCreateCtrl.filePath);
		OutputMsg(msg);
	}
	return result;
}
BOOL AbstractFileManager::doOpenFile()
{
	BOOL result = FALSE;
	while (1)
	{
		CXMLDOMFilePtr xmlDOMFile(new CXMLDOMFile());
		CXMLSOMFilePtr schemaFile = GetSchemaFile();
		DesBool desBool = xmlDOMFile->Open(m_xmlFilePath,schemaFile);
		if(!desBool.result)
		{
			CString err;
			err.Format(TRANS_T("�ļ�\"%s\"��ʧ�ܡ�\nԭ��DOM�ļ�����ʧ�ܡ�"),
				m_xmlFilePath);
			OutputErr(err);
			result = FALSE;	/* �ļ���ʧ�� */
			break;
		}
		m_xmlModelParser = GetModelParser();
		if (m_xmlModelParser == 0)
		{
			CString err;
			err.Format(TRANS_T("�ļ�\"%s\"��ʧ�ܡ�\nԭ�򣺽���������ʧ�ܡ�"),
				m_xmlFilePath);
			OutputErr(err);
			result = FALSE;	/* û���ļ������� */
			break;
		}
		Shared_XMLModel	xmlModel = m_xmlModelParser->ParseXMLDOM(xmlDOMFile);
		m_xmlModelParser.reset();
		if (xmlModel == 0)
		{
			CString err;
			err.Format(TRANS_T("�ļ�\"%s\"��ʧ�ܡ�\nԭ��ģ�ͽ���ʧ�ܡ�"),
				m_xmlFilePath);
			OutputErr(err);
			result = FALSE;	/* ����ʧ�� */
			break;
		}
		m_xmlModelMap.insert(std::make_pair(m_xmlFilePath,xmlModel));
		result = TRUE;
		break;
	}
	m_xmlFileOpened = result;
	if (result != TRUE)
	{
		m_xmlFileName.Empty();
		m_xmlFilePath.Empty();
	}
	else
	{
		CString msg;
		msg.Format(TRANS_T("�ļ�\"%s\"�򿪳ɹ���"),
			m_xmlFilePath);
		OutputMsg(msg);
	}
	return result;
}
BOOL AbstractFileManager::doCloseFile()
{
	CString filePath = m_xmlFilePath;
	m_xmlModelMap.clear();
	m_xmlFileName.Empty();
	m_xmlFilePath.Empty();
	m_xmlFileOpened = FALSE;
	CString msg;
	msg.Format(TRANS_T("�ļ�\"%s\"�رճɹ�."),
		filePath);
	OutputMsg(msg);
	return TRUE;
}
/*************************protected function******************************/
void AbstractFileManager::OutputMsg(CString msg)
{

}
void AbstractFileManager::OutputErr(CString err)
{

}
CXMLSOMFilePtr AbstractFileManager::GetSchemaFile()
{
	return NULL;
}
Unique_XMLModelParser AbstractFileManager::GetModelParser()
{
	Unique_XMLModelParser modelParser(new XMLModelParser());
	return modelParser;
}
Unique_FileThread	AbstractFileManager::GetFileThread()
{
	Unique_FileThread fileThread;
	return fileThread;
}

/*************************private function********************************/
