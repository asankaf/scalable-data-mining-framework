#include "StdAfx.h"
#include "C45TreeNominal.h"
#include "c45modelselection.h"
#include "c45pruneableclassifiertree.h"



C45TreeNominal::C45TreeNominal(void)
{
	m_unpruned = false;	
	m_collapseTree = true;
	m_CF = 0.25;	
	m_minNumObj = 2;	
	m_useMDLcorrection = true;			
	m_binarySplits = false;
	m_subtreeRaising = true;
	m_noCleanup = false;
	m_existence_map = NULL;
	m_source = NULL;
}

C45TreeNominal::~C45TreeNominal(void)
{
	if (m_existence_map != NULL)
	{
		delete m_existence_map;
	}
	if (m_source != NULL)
	{
		delete this->m_source;
	}	
}

void C45TreeNominal::CreateExistenceBitMap()
{
	assert(m_source != NULL);
	dynamic_bitset<> temp(m_source->numInstances());
	temp.flip();
	if (m_source->Type() == BitStreamInfo::EWAH_COMPRESSION)
	{
		m_existence_map = new EWAH();
	}else if (m_source->Type() == BitStreamInfo::WAH_COMPRESSION)
	{
		m_existence_map = new WAHStructure();
	}
	else if (m_source->Type() == BitStreamInfo::VERTICAL_STREAM_FORMAT)
	{
		m_existence_map = new VBitStream();
	}	
	m_existence_map->CompressWords(temp);
}

void C45TreeNominal::buildClassifier(WrapDataSource * instances)
{
	ModelSelection  * modSelection;	 	
	//Sets the source to be used
	m_source = new DataSource(instances,m_classIndex);
	m_source->Print();
	if (m_existence_map == NULL)
	{
		CreateExistenceBitMap();	
	}

	if (m_binarySplits)
	{
		//modSelection = new BinC45ModelSelection(m_minNumObj, instances, m_useMDLcorrection);
		int i = 0;
	}
	else{
		modSelection = new C45ModelSelection(m_minNumObj, m_source,m_existence_map, m_useMDLcorrection);
	}
	
		m_root = new C45PruneableClassifierTree(modSelection, !m_unpruned, m_CF,m_subtreeRaising, !m_noCleanup, m_collapseTree);
	
	m_root->buildClassifier(m_source,m_existence_map);

// 	if (m_binarySplits) {
// 		((BinC45ModelSelection)modSelection).cleanup();
// 	} else {
// 		((C45ModelSelection)modSelection).cleanup();
// 	}
}

