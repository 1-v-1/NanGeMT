
/**********************************************************
* ������:TEMPLATE.C                                       *
* ˵  ��:����й�ģ��Ĳ���                               *
* ʱ  ��:1996.7.4                                         *
**********************************************************/
/*********************************************************
* EDIT HISTORY                                           *
*   1996.7.4   written by CBB							 *											 *
*	1997.1.16  fix a bug in Template::free()			 *
*********************************************************/
#include "template.h"
//#include  <io.h>
#include <iostream>
#include <string.h>

/************************************
* ��������FinalTPL�ĳ�Ա��������
************************************/
void FinalTPL::free()
{
    AttrList.free();
}
void FinalTPL::combine(FinalTPL *tpl) //���ģ�����
{
	AttrVal *av1,*av2;    
	TargetTPL *ttp1,*ttp2;
	
	av2=tpl->getAttrList().getAttListHead();	
	while ( av2!=NULL )
	{

        av1=AttrList.IsAnAtt(av2);
		
		if(av1!=NULL) //av2ָ��������Ѿ�����
			if(strcmp(av1->getValue(),av2->getValue())!=0)
			{	
				if(strcmp(av1->getValue(),"")!=0 )
				{
					if (strcmp(av2->getValue(),"")!=0)
					{
						strcat(av1->getValue(),"|");
						strcat(av1->getValue(),av2->getValue());
					}
				}
				else
					av1->SetValue(av2->getValue());
			}
		if(av1==NULL) //av2ָ��������в�����
		{
			AttrVal * avtemp;
			avtemp = new AttrVal;
			avtemp->SetName(av2->getName());
			avtemp->SetValue(av2->getValue());
			avtemp->SetAttClass(av2->getAttClass());
			avtemp->SetAssAttName(av2->getAssAttName());
			avtemp->SetNext(NULL);	           
		    AttrList.AddAAttrVal(avtemp);
		}
		av2=av2->getNext();
	}
 //���´���Ŀ��ģ��
	 ttp2=tpl->getTarget().getTargetHead();
	 while (ttp2!=NULL)
	 {
	 	ttp1=Target.IsATarget(ttp2->getName());
	 	if (ttp1==NULL)
	 	{
	 		TargetTPL *ttp;
	 		ttp = new TargetTPL;
	 		ttp->setName(ttp2->getName());
	 		ttp->setNext(NULL);
	 		Target.AddATarget(ttp);
	 	}
	 	ttp2=ttp2->getNext(); 
	 }

}
void FinalTPL::compute(TPL *tpl)
{
	AttrVal *av1,*av2;
	TargetTPL *ttp1,*ttp2;

	while(strcmp(tpl->getName(),"")!=0) //��tplָ���ģ�岻�Ǹ��ڵ�ʱ,ִ��ѭ��
	{
		
		av2=tpl->getAttrList().getAttListHead();
		while (av2!=NULL)
		{
			av1=AttrList.IsAnAtt(av2);
			if(av1==NULL)                //��av2ָ��������в�����ʱ,�������
			{
				AttrVal * avtemp;                 //���ӶԹ������Ե�֧��
				avtemp = new AttrVal;
				avtemp->SetName(av2->getName());
				avtemp->SetValue(av2->getValue());
				avtemp->SetAssAttName(av2->getAssAttName());
				avtemp->SetAttClass(av2->getAttClass());
				avtemp->SetNext(NULL);	           
		    	AttrList.AddAAttrVal(avtemp);
		    }
		    av2=av2->getNext();
		 }
		 //���´���Ŀ��ģ��
		 ttp2=tpl->getTarget().getTargetHead();
		 while (ttp2!=NULL)
		 {
		 	ttp1=Target.IsATarget(ttp2->getName());
		 	if (ttp1==NULL)
		 	{
		 		TargetTPL *ttp;
		 		ttp = new TargetTPL;
		 		ttp->setName(ttp2->getName());
		 		ttp->setNext(NULL);
		 		Target.AddATarget(ttp);
		 	}
		 	ttp2=ttp2->getNext();
		 }
		 tpl=tpl->getParent();
	}
}

/************************************
* ��������AttrValList�ĳ�Ա��������
************************************/
AttrVal * AttrValList::IsAnAtt(AttrVal *av)
{
	AttrVal *tmp=getAttListHead();
	while (tmp!=NULL)
	{
		if(strcmp(tmp->getName(),av->getName())==0)
			if (strcmp(tmp->getAssAttName(),av->getAssAttName())==0)
			 	break;
		tmp=tmp->getNext();
	}
	return tmp;
}

char * AttrValList::GetValueOfAttribute(const char *szAttributeName)
{
	AttrVal *tmp=getAttListHead();
	while (tmp!=NULL)
	{
		if(strcmp(tmp->getName(),szAttributeName)==0) break;
		tmp=tmp->getNext();
	}
	if (tmp!=NULL) return tmp->getValue();
	else return NULL ;
}

void AttrValList::AddAAttrVal( AttrVal *av)
{
	AttrVal *tmp=getAttListHead();
	if (tmp==NULL)
	{
	    AttListHead=av;
	    return;
	 }
	 while(tmp->getNext()!=NULL) tmp=tmp->getNext();
	 tmp->SetNext(av); 
} 
void AttrValList::free()
{
	AttrVal *tmp;
	
	tmp=AttListHead;	
	while (AttListHead!=NULL)
	{
		tmp = AttListHead;
		AttListHead = AttListHead->getNext();
		delete tmp;
	}
}

/*************************************
* ��������TargetList�ĳ�Ա��������
*************************************/
int TargetList::DispAllTarget() //��ʾ���е�Ŀ��ģ�壬����Ŀ��ģ��ĸ���
{
	int count=0;
	TargetTPL *tgt=getTargetHead();
	while(tgt!=NULL)
	{
		count++;
		cout<<"\t"<<count<<"----"<<tgt->getName()<<endl;
		tgt=tgt->getNext();
	}	       
	return count;
}
char *TargetList::findTarget(int no) 
{
	int count=0;
	TargetTPL *tgt=getTargetHead();
	while(tgt!=NULL)
	{
		count++;
		if (count==no) return tgt->getName();
		tgt=tgt->getNext();
	}	       
	return "";
}
TargetTPL * TargetList::IsATarget(const char *Name)
{
	TargetTPL *tmp=getTargetHead();
	while (tmp!=NULL)
	{
		if(strcmp(tmp->getName(),Name)==0) break;
		tmp=tmp->getNext();
	}
	return tmp;
}

void TargetList::AddATarget( TargetTPL *ttpl)
{
	TargetTPL *tmp=getTargetHead();
	if (tmp==NULL) 
	{
		TargetHead=ttpl;
		return;
	}
	while (tmp->getNext()!=NULL) tmp=tmp->getNext();
	tmp->setNext(ttpl);
}
void TargetList::free()
{
	TargetTPL *ttpl;

	ttpl=TargetHead;	
	while (TargetHead!=NULL)
	{
		ttpl = TargetHead;
		TargetHead = TargetHead->getNext();
		delete ttpl;
	}
}


/**************************************
* ��������TPL�ĳ�Ա�����Ķ���
**************************************/

int TPL::DispAllChild()  			//��ʾ��ģ���������ģ��
{									//����ֵ����ģ��ĸ���
	int i=0;
	
	TPL *tmp=firstChild;
	
/*	cout<<Name<<endl;
	cout<<endl;*/
	while(tmp!=NULL)
	{
		i++;
		cout<<'\t'<<i<<"---"<<tmp->getName()<<endl;
		tmp=tmp->getNextBrother();
	}
	return i;
}	
char * TPL::findChild(int no)  	
{								
	int i=0;
	
	TPL *tmp=firstChild;
	
	while(tmp!=NULL)
	{
		i++;
		if (i==no) return tmp->getName();
		tmp=tmp->getNextBrother();
	}
	return "";
}
	

void TPL::AddAChild(  TPL * tpntr) //Ϊ��ģ������һ����ģ��
{
     if ( firstChild==NULL )
     {
     		firstChild = tpntr;
      		return;
      }
      else
      {
       		TPL *tmp = firstChild;
       		tmp->AddABrother(tpntr);
       		return;
       }
}

void TPL::AddABrother( TPL *tpntr) //Ϊ��ģ������һ���ֵ�ģ��
{
   	if ( nextBrother==NULL)
   	{
   		nextBrother = tpntr;
   		return;
   	}
   	else
   	{
   		TPL *tmp=nextBrother;
   		tmp->AddABrother(tpntr);
   		return;
   	}
}

void TPL::free()
{
     FinalTPL::free();
     Target.free();
}



/**************************************************
* ��������Template�ĳ�Ա�����Ķ���
**************************************************/

/***********************************************************
* ���ݲ���ָ����ģ���������Դ��뵽��Ӧ��ģ�����в��Ҹ�ģ��
* ���ҵ�������ָ���ģ���ָ�룬���򣬷���NULL
***********************************************************/

int  Template::DispChild(int Lang,char *tplname)
{
 	TPL *tmp1;
 	tmp1=find(tplname,Lang);
 	if (tmp1==NULL) 
 	{
 		cout<<"ָ����ģ�岻����!"<<endl;
 		return 0;
 	}
    return tmp1->DispAllChild();

}
char * Template::findChild(const char *tplName,int LangCode,int no)
{
	TPL *tmp1;
	tmp1=find(tplName,LangCode);

	return tmp1->findChild(no);
}
TPL * Template::find(const char *tplName,int LangCode)
{
	TPL *tmp1,*tmp2;
	tmp2=&TplTree[LangCode];
	tmp1=preOrder(tplName,tmp2);  //��������������̼���ָ����ģ��
	return tmp1;

}

/*****************************************************
* �������򷽷�����ģ������������ָ����ģ��
* 	������ʧ�ܣ�����NULL
* 	�������ɹ�������ָ��ָ��ģ���ָ��
*****************************************************/
TPL * Template::preOrder(const char *tplName,TPL * tpl)
{
	if (strcmp(tplName,tpl->getName())==0)
		return tpl;
	if (tpl->getFirstChild()==NULL && tpl->getNextBrother()==NULL)
		return NULL;
	if (tpl->getFirstChild()!=NULL)
	{
		TPL *tmp1;
		tmp1=preOrder(tplName,tpl->getFirstChild());
		if (tmp1!=NULL) return tmp1;	
	}
	if (tpl->getNextBrother()!=NULL)
	{
		TPL *tmp2;
		tmp2=preOrder(tplName,tpl->getNextBrother());
		return tmp2;
	}
	return NULL;
	
}

/*************************************************
* ��ʾģ����
*************************************************/
void printTree(TPL * tmp,int l)
{
	int i=1;
	for (;i<l;++i) cout<<"|   ";
	for (;i<=l;++i) cout<<"+-- ";
	if ( strcmp(tmp->getName(),""))
		cout<<tmp->getName();
	else
		cout<<"ROOT";
	cout<<endl;
	if ( tmp->getFirstChild()!=NULL )
		printTree(tmp->getFirstChild(),l+1);
	if ( tmp->getNextBrother()!=NULL )
		printTree(tmp->getNextBrother(),l);
}
TPL *Template::TreeAddress(int Langcode)
{
	return &TplTree[Langcode];
}
void Template::printout()
{
	TPL *tmp2;
	for(int i=0;i<LANGNUM;i++)
	{
		tmp2=&TplTree[i];
		printTree(tmp2,0); 
	}
}

/************************************************
* ���ģ���ļ�(TXT�ļ�)
************************************************/

#ifdef KBMS
/************************************************
* ��tplbase.TXT��ĳ�����Ե�����ģ�����TxtBuff  *
* �������е�ע����Ϣ�����Ѷ���ո�ѹ��Ϊһ���ո�*
************************************************/
BOOL Template::rdTxtTpl()
{
	return rdTxtRecord("BeginTemplate","EndTemplate");
}
/****************************************************
* ��Template.TXT�ļ�ת��ΪTemplate.DAT�ļ�
* ͬʱ���ڴ潨����ģ�����ṹ 
*****************************************************/

void Template::Load()
{
	
	wopen(); // ��д��ʽ��tplbase/tplbase.DAT�ļ�
	if( getStatus()!=WOPEN) 
		error("Template::Load(): cannot load while Template not open");

	beginScan(MAXBUFFER);

	char txtfilename[FILENAMELENGTH];
	getFullName(txtfilename,".TXT");
	/* ����getPath(),getName()�Լ��ṩ�ĺ�׺����������·����
	   �� tplbase/tplbase.TXT */
	
	/* �Զ���ʽ��tplbase.TXT �ļ�*/
	TxtFile.open(txtfilename,ios::in|ios::binary);
	if( !TxtFile ) error("Template::Load(): TXTOPEN",txtfilename);

	CErrorException * err=NULL;
	try
	{
		int i;
		IOToken TheToken;

		BOOL flag;
		flag=TRUE;
		
		/*��tplbase.TXT��ĳ�����Ե�����ģ�����TxtBuff */
		while(flag=rdTxtTpl())
		{       
			TheToken.setInPntr(TxtBuff);
			TheToken.setOutPntr(DatBuff);
			try
			{
				i=LoadTPLs(TheToken);
			}
			catch (CErrorException *)
			{
				message("Template:",TxtBuff);
				throw;
			}
			wtDatLength(i);
			wtDatBuff();
		}
	}
	catch (CErrorException * e) { err=e; }

	TxtFile.close();

	endScan();
	close();
#ifdef _UNIX
	printout();
#endif
	setBuilt(YES);
	free();

	if (err) throw err;
}

/****************************************************
* ��ĳ�����Ե�����ģ���TxtBuff����DatBuff          *
* ���ڴ��н���������Ե�ģ����                      *
****************************************************/
int Template::LoadTPLs(IOToken &TheToken)
{
	char *TempPntr;
	TempPntr=TheToken.getOutPntr();
	TheToken.input();
	if(!TheToken.isEqualTo("BeginTemplate")) 
		error("Template::LoadTPLs(): Need \"BeginTemplate\"",TheToken.getBuffer());

	TheToken.output();  //��"BeginTemplate"�����DatBuff
	TheToken.input();

	int lang;
	lang=LoadLangCode(TheToken);//���뵱ǰ���ڴ����ģ������Դ���
	TheToken.output('\n');	
	LoadTPLsBody(TheToken,lang);

	if(!TheToken.isEqualTo("EndTemplate")) 
		error("Template::LoadTPLs(): Need \"EndTemplate\"",TheToken.getBuffer());

	TheToken.output(); //��"EndTemplate"�����DatBuff
	TheToken.output('\n');
	TheToken.output('\n');
	int num=TheToken.getOutPntr()-TempPntr; //�������������ģ��ĳ���
	TheToken.outputNull();
	return num;//���س���
}

/******************************************************
* ��"{��������}"��TxtBuff�����DatBuff
* ��ɴ��������Ƶ����Դ����ת������
* ������������Դ���
*******************************************************/
int	 Template::LoadLangCode(IOToken &TheToken)
{
	if(!TheToken.isEqualTo("{"))  //��ǰTOKEN��"{"��?
		error("Template::LoadLangCode(): Need \"{\"",TheToken.getBuffer());

	TheToken.output(' '); //����ո�DatBuff,��ָ�����
	TheToken.output();   //�����ǰTOKEN��DatBuff,��"{"
	TheToken.input();

	int i;  //�������Ƶ����Դ����ת��
	for(i=0;i<LANGNUM;i++) if(TheToken.isEqualTo(LANGSTRING[i])) break;
	if(i>=LANGNUM) error("Template::LoadLangCode(): Invalid LangCode");

	TheToken.output();
	TheToken.input();

	if(!TheToken.isEqualTo("}")) 
		error("Template::LoadLangCode(): Need \"}\"",TheToken.getBuffer());

	TheToken.output();
	TheToken.input();
	return i;//�������Դ���
}

/*****************************************************
* װ��ĳ�����Ե�����ģ��                             *
* ����˺���ʱ,TxtBuffӦΪ"Name..."                  *
*****************************************************/
void Template::LoadTPLsBody(IOToken &TheToken,int lang)
{
     
     while ( TheToken.isEqualTo("Name") )  //ÿһ��ѭ������һ��ģ��
     	LoadATPL( TheToken,lang);          //װ��һ��ģ��
}
/*********************************************************
* װ��ģ���ļ��е�һ��ģ�壬�������ģ����               *
* ����˺���ʱ,TxtBuffӦΪ"Name..."                      *
*********************************************************/
void Template::LoadATPL(IOToken &TheToken,int lang)
{
	
	TPL *tpl;
	
	tpl=new TPL;

	loadName(TheToken,tpl);
	loadParent(TheToken,tpl,lang);
	loadTarget(TheToken,tpl);
	loadAttVal(TheToken,tpl);
}
/*******************************************
* װ��Name�� 	                           *
* ����˺���ʱ,TxtBuffӦΪ"Name..."        *
*******************************************/
void Template::loadName(IOToken &TheToken,TPL *tpl)
{

	if (!TheToken.isEqualTo("Name"))   
		error("Template::loadName(): Need \"Name\"",TheToken.getBuffer());
	TheToken.output('\t');
	TheToken.output();  //���"Name"
	
	TheToken.input();	//����":"
	if (!TheToken.isEqualTo(":"))   
		error("Template::loadName(): Need \":\"",TheToken.getBuffer());
	TheToken.output();   //���":"
	
	TheToken.input(); //����ģ���� 
	if (TheToken.getType()!=IDENT)
		error("Template::loadName(): Template Name must be a string");
	tpl->setName(TheToken.getBuffer());
	TheToken.output(); //���ģ����
	TheToken.output('\n');

	TheToken.input(); //����"Parent"��"Target"
}
/**********************************************************
* װ��Parent��                                            *
* ����˺���ʱ,TxtBuffӦΪ"Parent..."��"Target..."        *
**********************************************************/
void Template::loadParent(IOToken &TheToken,TPL *tpl,int lang)
{
 	if (!TheToken.isEqualTo("Parent")&&!TheToken.isEqualTo("Target"))   
		error("Template::loadParent(): Need \"Parent\"or \"Target\"",TheToken.getBuffer());
	if ( TheToken.isEqualTo("Parent"))
	{
		TheToken.output('\t');	
		TheToken.output(); //���"Parent"
		
		TheToken.input();  //����":"
		if ( !TheToken.isEqualTo(":"))
			error("Template::loadParent(): Need \":\"",TheToken.getBuffer());
		TheToken.output();
		
		TheToken.input();  //���븸ģ����
		if (TheToken.getType()!=IDENT)
			error("Template::loadParent(): Parent Name must be a string");
		TPL * tmp=NULL;
		tmp=find(TheToken.getBuffer(),lang);//�ڸ����Ե�ģ�����м�����ģ��
		if ( tmp==NULL )
			error("Template:loadParent():Parent Template does not exist!",TheToken.getBuffer());
		tpl->setParent(tmp);
		tmp->AddAChild(tpl);//�Ѹ�ģ����Ϊ��ģ���һ����ģ����븸ģ��
		TheToken.output();
		TheToken.output('\n');
		
		TheToken.input();//����"Target"
	}
	else
	{      
	 //����ģ�����ģ����(�޸�ģ������)
		TPL * tmp=NULL;
		tmp=find("",lang);//�ڸ����Ե�ģ�����м�����ģ��
		if ( tmp==NULL )
			error("Template::loadParent():Virtual Parent Template does not exist!","");
		tpl->setParent(tmp);
		tmp->AddAChild(tpl);//�Ѹ�ģ����Ϊ��ģ���һ����ģ����븸ģ��
	}
}
/*********************************************
* װ��Target��                               *
* ����˺���ʱ,TxtBuffӦΪ"Target..."        *
*********************************************/
void Template::loadTarget(IOToken &TheToken,TPL *tpl)
{
	if(!TheToken.isEqualTo("Target"))
		error("Template::loadTarget(): Need \"Target\"",TheToken.getBuffer());
	TheToken.output('\t');
	TheToken.output();//���Target
	
	TheToken.input();//����":"
	if (!TheToken.isEqualTo(":"))   
		error("Template::loadTarget(): Need \":\"",TheToken.getBuffer());
	TheToken.output();
	
	TheToken.input();//����"{"��"BEGIN"
	
	int i;
	while (TheToken.isEqualTo("{")) //ÿѭ��һ����Ŀ��ģ�����м���һ��Ŀ��ģ��
	{     
		
		i=LoadLangCode(TheToken);
		if(TheToken.getType()!=IDENT )
			error("Template::loadTarget(): Target Name must be a string");
		TargetTPL *ttpl=new TargetTPL;
		ttpl->setName(TheToken.getBuffer());
		ttpl->setLangCode(i);
		tpl->getTarget().AddATarget(ttpl);
		TheToken.output();//���ģ����
		TheToken.input();//����","��"BEGIN"
		if (!TheToken.isEqualTo(",")&&!TheToken.isEqualTo("BEGIN")) 
			error("Template::loadTarget(): Need \",\"or \"BEGIN\"",TheToken.getBuffer());
		if (TheToken.isEqualTo(",")) 
		{	
			TheToken.output();//���","
			TheToken.input();//����"{",׼��������һĿ��ģ��
			if (!TheToken.isEqualTo("{"))   
				error("Template::loadTarget(): Need \"{\"",TheToken.getBuffer());
		}
	}
	TheToken.output('\n');
}
/*********************************************
* װ���������ֵ                             *
* ����˺���ʱ,TxtBuffӦΪ"BEGIN..."         *			
*********************************************/
void Template::loadAttVal(IOToken &TheToken,TPL *tpl)
{               
	char szAssName[MAXSTRLENGTH];
	strcpy(szAssName,"");

	if (!TheToken.isEqualTo("BEGIN"))   
		error("Template::loadAttval(): Need \"BEGIN\"",TheToken.getBuffer());
	TheToken.output('\t');
	TheToken.output(); 		//���"BEGIN"
	TheToken.output('\n');
	TheToken.input();  		//�������������,��:"DistinguishAttribute"
	Attribute * attr;
	Value * val;
	int nCurAttClass=0;
	int nIsAssAtt=0;
	
	//һ��ѭ������һ������
	while ( !TheToken.isEqualTo("END"))
	{
        if (!TheToken.isEqualTo("DistinguishAttribute")
        	&&!TheToken.isEqualTo("NoValueAttribute")
        	&&!TheToken.isEqualTo("CommonAttribute"))
        	error("Template:loadAttVal():Need\"DistinguishAttribute\"or\"NoValue"
        		"Attribute\"or\"CommonAttribute\"",TheToken.getBuffer());
        
        if (TheToken.isEqualTo("DistinguishAttribute"))
        	nCurAttClass=AC_DISTINGUISH;	
        if (TheToken.isEqualTo("NoValueAttribute"))
        	nCurAttClass=AC_NOVALUE;
        if (TheToken.isEqualTo("CommonAttribute"))
        	nCurAttClass=AC_COMMON;
        TheToken.output('\t');
        TheToken.output('\t');
        TheToken.output();
        TheToken.output('\n');
        TheToken.input();     //������������AssAtttibute
        while (!TheToken.isEqualTo("DistinguishEnd")
        	&&!TheToken.isEqualTo("CommonEnd")
        	&&!TheToken.isEqualTo("NoValueEnd"))
        {	
			if (TheToken.isEqualTo("AssAttribute"))
			{	
				nIsAssAtt = AC_ASSOCIATE;
				TheToken.output('\t');
				TheToken.output('\t');
				TheToken.output('\t');
				TheToken.output(); //���AssAttribute
				TheToken.input();  //�������������
				attr = NULL;
				attr = mkModel->getAttribute(TheToken.getBuffer());
				if (attr == NULL)
					error("Template::loadAttVal():Undefined Attribute",
						TheToken.getBuffer());
				strcpy(szAssName,TheToken.getBuffer());
				TheToken.output(' ');
				TheToken.output(); //���������
				TheToken.input();  //����"with"
				if (!TheToken.isEqualTo("with"))
					error("Template::loadAttVal():Need \"with\"",
					TheToken.getBuffer());
				TheToken.output(' ');
				TheToken.output(); //���"with"
				TheToken.output('\n'); //���"with"
				TheToken.input();  //����������
			}
			AttrVal *av = new AttrVal;	

			//��������������ģ�����Ƿ��Ѿ�����?
			attr=NULL;
			attr=mkModel->getAttribute(TheToken.getBuffer());
			if (attr==NULL)
				error("Template::loadAttVal():Undefined Attribute ",
					TheToken.getBuffer());
		
			
			av->SetAssAttName(szAssName);
			av->SetName(TheToken.getBuffer());
			av->SetAttClass(nCurAttClass|nIsAssAtt);
			TheToken.output('\t');
			TheToken.output('\t');
			TheToken.output('\t');
			if ( nIsAssAtt == AC_ASSOCIATE ) TheToken.output('\t');
			TheToken.output();		//���������
			TheToken.input();       //����":"��","
		
			if (av->IsDistinguish()&&!TheToken.isEqualTo(":"))
				error("Template::loadAttVal():Distinguish Attribute must have value!",
					av->getName());
			if (av->IsCommon()&&!TheToken.isEqualTo(":"))
				error("Template::loadAttVal():Common Attribute must have value!",
					av->getName());
			if (av->IsNoValue()&&TheToken.isEqualTo(":"))
				error("Template::loadAttVal():No Value Attribute must not have value!",
					av->getName());
					
			if (!TheToken.isEqualTo(":")
				&&!TheToken.isEqualTo(",")
				&&!TheToken.isEqualTo("DistinguishEnd")
				&&!TheToken.isEqualTo("CommonEnd")
				&&!TheToken.isEqualTo("NoValueEnd")
				&&!TheToken.isEqualTo("AssEnd")) 
				error("Template::loadAttVal(): Need \":\"or \",\"or\"Disting"
					"uishEnd\"or\"NoValueEnd\"or\"CommonEnd\"or\"AssEnd\"",
					TheToken.getBuffer());
			if ( TheToken.isEqualTo(":"))
			{
				TheToken.output();//���":"
				TheToken.input(); //����ֵ
				val=NULL;
				if ( attr->getLimit()==1 )
				{
					if ( attr->getNameAlias()!=1)
						val=attr->getValList().find(TheToken.getBuffer());
						//����ֵ�Ƿ�����ԵĺϷ�ֵ?
					else
						val=attr->getRefer()->getValList().find(TheToken.getBuffer());
						//����ֵ�Ƿ�����ԵĺϷ�ֵ?
					if ( val == NULL )
						error("Template::loadAttVal():Undefined Value ",
							TheToken.getBuffer());
				}
		    	av->SetValue(TheToken.getBuffer());
				TheToken.output();//���ֵ
				//����","��"DistinguishEnd"or"CommonEnd"or"NoValueEnd"
				TheToken.input(); 
				if (!TheToken.isEqualTo(",")
					&&!TheToken.isEqualTo("DistinguishEnd")
					&&!TheToken.isEqualTo("CommonEnd")
					&&!TheToken.isEqualTo("NoValueEnd")
					&&!TheToken.isEqualTo("AssEnd")) 
					error("Template::loadAttVal(): Need \",\"or\"DistinguishEnd"
						"\"or\"CommonEnd\"or\"NoValueEnd\"",TheToken.getBuffer());
			}
			tpl->getAttrList().AddAAttrVal(av);
			if (TheToken.isEqualTo("AssEnd"))
			{
				nIsAssAtt = 0;
				strcpy(szAssName,""); 
				TheToken.output('\n');
				TheToken.output('\t');
				TheToken.output('\t');
				TheToken.output('\t');
				TheToken.output();//���"AssEnd"
				TheToken.input(); //�����̷��ţ��磺",","DistinguishEnd"��
				if (!TheToken.isEqualTo(",")
					&&!TheToken.isEqualTo("DistinguishEnd")
					&&!TheToken.isEqualTo("CommonEnd")
					&&!TheToken.isEqualTo("NoValueEnd")	) 
					error("Template::loadAttVal(): Need \",\"or\"DistinguishEnd"
						"\"or\"CommonEnd\"or\"NoValueEnd\"",TheToken.getBuffer());
			}
			
			if (TheToken.isEqualTo(","))
			{
				TheToken.output(); //���","
				TheToken.input();  //������һ����
			}
			if (TheToken.isEqualTo("DistinguishEnd")
				||TheToken.isEqualTo("CommonEnd")
				||TheToken.isEqualTo("NoValueEnd"))
				nCurAttClass = 0;
			TheToken.output('\n');
		}
		TheToken.output('\t');
		TheToken.output('\t');
		TheToken.output(); // ���"DistinguishEnd"��"CommonEnd"��"NoValueEnd"
		TheToken.output('\n');
		TheToken.input();  // ����"DistinguishAttribute""END"
	}
	TheToken.output('\t');
	TheToken.output(); // ���"END"
	TheToken.output('\n');
	TheToken.input();//����"Name"��"EndTemplate"
}
#endif // KBMS
			
void Template::build()
{
	if (getBuilt()==YES ) return;
	ropen();                      //�Զ���ʽ��tplbase/tplbase.DAT
	beginScan(MAXBUFFER);

	try
	{
		int leng;          
		while(leng=rdDatLength()){
			rdDatBuff(leng);
			IToken TheToken(DatBuff);
			ReadTPLs(TheToken);
		}
	}
	catch (CErrorException *) { endScan(); close(); throw; }

	endScan();                  
	close();                    //�ر�tplbase/tplbase.DAT�ļ�
	setBuilt(YES);
	MKbase::build();
}

void Template::ReadTPLs(IToken &TheToken)
{		
	TheToken.input();           //����"BeginTemplate"

	TheToken.input();           //����"{"

	int lang;
	lang=ReadLangCode(TheToken);//���뵱ǰ���ڴ����ģ������Դ���
	ReadTPLsBody(TheToken,lang);

}		
		
int	Template::ReadLangCode(IToken &TheToken)
{
	TheToken.input();	// ������������
	int i;
	for(i=0;i<LANGNUM;i++) if(TheToken.isEqualTo(LANGSTRING[i])) break;
	TheToken.input();	// ����"}"
	TheToken.input();	// ����"Name"
	return i;
}
		
void Template::ReadTPLsBody(IToken &TheToken,int lang)
{		
     
     while ( TheToken.isEqualTo("Name") )  //ÿһ��ѭ������һ��ģ��
     	ReadATPL( TheToken,lang);          //װ��һ��ģ��
}
void Template::ReadATPL(IToken &TheToken,int lang)
{
	TPL *tpl;
	
	tpl=new TPL;

	ReadName(TheToken,tpl);
	ReadParent(TheToken,tpl,lang);
	ReadTarget(TheToken,tpl);
	ReadAttVal(TheToken,tpl);
}
void Template::ReadName(IToken &TheToken,TPL *tpl)
{
    //��ʱ"Name"���ڻ�������	
	TheToken.input();	//����":"
	TheToken.input();   //����ģ���� 

	tpl->setName(TheToken.getBuffer());
	
	TheToken.input();   //����"Parent"��"Target"
}
void Template::ReadParent(IToken &TheToken,TPL *tpl,int lang)
{ 
	//��ʱTOKEN����������"Parent"��"Target"
	
	if ( TheToken.isEqualTo("Parent"))
	{
		TheToken.input();  //����":"
		TheToken.input();  //���븸ģ����

		TPL * tmp=NULL;
		tmp=find(TheToken.getBuffer(),lang);//�ڸ����Ե�ģ�����м�����ģ��
		if ( tmp==NULL )
			error("Template::ReadParent():Parent Template does not exist!",TheToken.getBuffer());

		tpl->setParent(tmp);
		tmp->AddAChild(tpl);//�Ѹ�ģ����Ϊ��ģ���һ����ģ����븸ģ��
		
		TheToken.input();//����"Target"
	}
	else
	{      
	 //����ģ�����ģ����(�޸�ģ������)
		TPL * tmp=NULL;
		tmp=find("",lang);//�ڸ����Ե�ģ�����м�����ģ��
		if ( tmp==NULL )
			error("Template::ReadParent():Virtual Parent Template does not exist!","");
		tpl->setParent(tmp);
		tmp->AddAChild(tpl);//�Ѹ�ģ����Ϊ��ģ���һ����ģ����븸ģ��
	}
}
void Template::ReadTarget(IToken &TheToken,TPL *tpl)
{
	//��ʱTOKEN��������Ϊ"Target"
	TheToken.input();//����":"
	TheToken.input();//����"{"��"BEGIN"
	
	int i;
	while (TheToken.isEqualTo("{")) //ÿѭ��һ����Ŀ��ģ�����м���һ��Ŀ��ģ��
	{     
		
		i=ReadLangCode(TheToken);
        //��ʱTOKEN��������ΪĿ��ģ������
		TargetTPL *ttpl=new TargetTPL;
		ttpl->setName(TheToken.getBuffer());
		ttpl->setLangCode(i);
		tpl->getTarget().AddATarget(ttpl);

		TheToken.input();//����","��"BEGIN"
		if (TheToken.isEqualTo(",")) TheToken.input();//����"{",׼��������һĿ��ģ��
	}
}




void Template::ReadAttVal(IToken &TheToken,TPL *tpl)
{                                                 
 
	char szAssName[MAXSTRLENGTH];
	strcpy(szAssName,"");
	int nCurAttClass = 0;
	int nIsAssAtt = 0;
	//��ʱTOKEN��������Ϊ"BEGIN"
	TheToken.input();  		//�������������
	Attribute * attr;
	while ( !TheToken.isEqualTo("END"))
	{
        if (TheToken.isEqualTo("DistinguishAttribute"))
        	nCurAttClass=AC_DISTINGUISH;	
        if (TheToken.isEqualTo("NoValueAttribute"))
        	nCurAttClass=AC_NOVALUE;
        if (TheToken.isEqualTo("CommonAttribute"))
        	nCurAttClass=AC_COMMON;
 	    TheToken.input(); //������������"AssAttribute" 
        while (!TheToken.isEqualTo("DistinguishEnd")
        	&&!TheToken.isEqualTo("CommonEnd")
        	&&!TheToken.isEqualTo("NoValueEnd"))
        {	
			if (TheToken.isEqualTo("AssAttribute"))
			{	
				nIsAssAtt = AC_ASSOCIATE;
				TheToken.input();  //�������������
				attr = NULL;
				attr = mkModel->getAttribute(TheToken.getBuffer());
				if (attr == NULL)
					error("Template::ReadAttVal():Undefined Attribute",
						TheToken.getBuffer());
				strcpy(szAssName,TheToken.getBuffer());
				TheToken.input();  //����"with"
				TheToken.input();  //����������
			}

			attr=NULL;
			attr=mkModel->getAttribute(TheToken.getBuffer());//��������������ģ�����Ƿ��Ѿ�����?
			if (attr==NULL)
				error("Template::RaedAttVal():Undefined Attribute ",
					TheToken.getBuffer());
		
			AttrVal *av = new AttrVal;       
			av->SetAssAttName(szAssName);
			av->SetName(TheToken.getBuffer());
			av->SetAttClass(nCurAttClass|nIsAssAtt);
		
			TheToken.input();       //����":"��","
		
			if ( TheToken.isEqualTo(":"))
			{
				TheToken.input(); //����ֵ
		    	av->SetValue(TheToken.getBuffer());
				TheToken.input(); //����","��
			}
			tpl->getAttrList().AddAAttrVal(av);
			if (TheToken.isEqualTo("AssEnd"))
			{
				nIsAssAtt = 0;
				strcpy(szAssName,""); 
				TheToken.input(); //�����̷��ţ��磺",","DistinguishEnd"��
			}
			if (TheToken.isEqualTo(",")) TheToken.input();  //������һ����
			if (TheToken.isEqualTo("DistinguishEnd")
				||TheToken.isEqualTo("CommonEnd")
				||TheToken.isEqualTo("NoValueEnd"))
				nCurAttClass = 0;
		}
		TheToken.input();//����"DistinguishAttribute"��"END"
	}
	TheToken.input();//����"Name"��"EndTemplate"
}

void treefree(TPL *tpl);
void Template::free()   		             //�ͷ�ģ�����ṹ��������ڴ�ռ�
{
	if(getBuilt()==NO) return;
	TPL * tpl;
	int lang;
	for (lang=0;lang<LANGNUM;lang++)
	{
		tpl=TplTree[lang].getFirstChild();
		if(tpl!=NULL) treefree(tpl);
		TplTree[lang].setFirstChild((TPL*)NULL);

	}    
	MKbase::free();
}
void treefree(TPL *tpl)
{
	TPL *left,*right;
	
	left=tpl->getFirstChild();
	right=tpl->getNextBrother();
	
	if (left!=NULL)
	{
		treefree(left);
		tpl->setFirstChild((TPL*)NULL);
	}
	if (right!=NULL)
	{
		treefree(right);
		tpl->setNextBrother((TPL*)NULL);
	}
	tpl->free();
	delete tpl;
}
	
