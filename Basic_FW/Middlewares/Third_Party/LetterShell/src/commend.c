/*
 * commend.c
 *
 *  Created on: 2023��3��2��
 *      Author: Administrator
 */

#include "stdio.h"
#include <stdlib.h>
#include <string.h>
#include "flashdb.h"
#include "commend.h"
#include "shell.h"
#include "pid.h"
#include "log.h"

var_list_t var_list;
var_list_t* _var_begin = 0;



int a = -1234567890;
int a1 = 0;
char b = 1;
double c = -1.123456789;
float d = 1;
#define __NAME(restrc,x,name) strcpy(name,#x)

//���������������ӱ���ʹ�ñ���������������ʵʱ�鿴���޸�
//ע�᷽ʽ���£���һ����������ע�᷽ʽ��ͬ
//ʹ�ã�   var �� /var all  �г�����ע�����
//        var a         �鿴����a��ֵ
//        var a 1.2     �޸ı���a��ֵΪ1.2
//Ĭ�ϱ�����Ϊ�з������ͣ��������ʹ�С��������ʾ����
void var_init(void)
{

  //����ʹ��
    //ע�������������������С
    VAR_CMD_REGISTER(b, _8);
    VAR_CMD_REGISTER(a1, _32);
    VAR_CMD_REGISTER(c, _lf);//double����
    VAR_CMD_REGISTER(d, _f);//float����

//    var_register(&gps_tau1201.latitude,"curlat",_lf);
//    var_register(&gps_tau1201.longitude,"curlon",_lf);

//������aע��
    VAR_CMD_REGISTER(a, _32);//4�ֽڵ�
	//extern pid_t TripodHead_pid;
	extern float pos_x;
	extern float pos_y;
	extern float target;
	//var_register(&TripodHead_pid.p,"kp",_f);
	//var_register(&TripodHead_pid.i,"ki",_f);
	//var_register(&TripodHead_pid.d,"kd",_f);
	var_register(&pos_x,"pos_x",_f);
	var_register(&pos_y,"pos_y",_f);
	var_register(&target,"t",_f);
       // gps_latitude[i] = gps_control.gps_location.latitude_temp[i];
        //gps_longitude[i] = gps_control.gps_location.longitude_temp[i];
//    var_arr_register(gps_control.gps_location.latitude_temp,"gpslat",_lf,max);
//    var_arr_register(gps_control.gps_location.longitude_temp,"gpslon",_lf,max);

}
int DB_Get_API(int argc, char** argv);
int DB_Set_API(int argc, char** argv);
int DB_Co_API(int argc, char* name, char *KV);

//����һ����
//����2 ������
//����3 ����ֵ//��ѡ
void var(int argc, char** argv)
{
    var_list_t* _var = _var_begin;
    var_list_t* _var_second=0;
    int is_assign=0;
    //logInfo("var enter\r\n",0);
    if(argc==1)//���������1����ô��ӡ�����е�����
    {
        logInfo("\r\n");
       while (_var)
       {
                logInfo("\t-- %s \r\n", _var->name);
              _var = _var->next;

       }
       return;

    }

    if (argc > 4)//�����������Ϊ4--�д���
		{
        logInfo("paramer error\r\n");
        return;
    }

    if(0==strcmp(argv[1], "all"))//����ڶ����ַ�����ֵ��all
		{

        while (_var)
        {
            logInfo("\t--%s \r\n", _var->name);
            _var = _var->next;

        }
        return;
    }

    while (0!=strcmp(argv[1], _var->name))//ѭ���ж������Ƿ���ڹؼ���
    {

        _var = _var->next;
        if (_var == 0)
        {
            logInfo("There's no variable here\r\n");
            return;
        }//δ�ҵ�����
    }//�Ѿ��ҵ�����
    if(argc==4)//�ҵ�֮������жϣ����������Ƿ�Ϊ4
		{

        if(strcmp(argv[2],"=")==0){//����������ı����Ƿ�Ϊ����
                if(argc==4){
                    _var_second=_var_begin;
                    while (0!=strcmp(argv[3], _var_second->name))//����ṹ�жϹؼ���
                    {

                        _var_second = _var_second->next;
                        if (_var_second == 0)
                        {
                            logInfo("There's no variable here(var tow)\r\n");
                            return;
                        }//δ�ҵ�����
                    }//�Ѿ��ҵ�����
                    is_assign=1;
                }

            }
    }else is_assign=0;



    switch (_var->type)//���ڶԹؼ��ֽ���ƥ��
    {

    case _8://�����8λӦ�ô�ӡ������Ķ�Ӧ��ֵ�����¶���
        if(argc == 3 && is_assign==0){
            *(char*)_var->body = (char)atoi(argv[2]);
        }else if(is_assign==1){
            *(char*)_var->body = *(char*)_var_second->body;
        }
        logInfo("%s is %d\r\n", _var->name, *(char*)_var->body);
        break;
    //case _8:
    //    logInfo("%s is %d", _var->name, *(unsigned char*)_var->body);

    //    break;
    case _16:
        if(argc == 3&& is_assign==0){
            *(short*)_var->body = (short)atoi(argv[2]);
        }else if(is_assign==1){
            *(short*)_var->body = *(short*)_var_second->body;
        }
        logInfo("%s is %d\r\n", _var->name, *(short*)_var->body);

        break;
    //case _u16:
    //    logInfo("%s is %d", _var->name, *( unsigned short*)_var->body);

    //    break;
    case _32:
        if(argc == 3&& is_assign==0){
            *(int*)_var->body = atoi(argv[2]);
        }else if(is_assign==1){
            //logInfo("test\r\n",0);

            *(int*)_var->body = *(int*)_var_second->body;
        }
        logInfo("%s is %d\r\n", _var->name, *(int*)_var->body);

        break;
    //case _u32:
    //    logInfo("%s is %d", _var->name, *(unsigned int*)_var->body);
    //    break;
    case _64:
        if(argc == 3&& is_assign==0){
            *(long long*)_var->body = atoi(argv[2]);
        }else if(is_assign==1){
            *(long long*)_var->body = *(long long*)_var_second->body;
        }
        logInfo("%s is %d\r\n", _var->name, *(long long*)_var->body);

        break;
    //case _u64:
    //    logInfo("%s is %d", _var->name, *(unsigned long long*)_var->body);

    //    break;
    case _f:
        if(argc == 3&& is_assign==0){
            *(float*)_var->body = (float)atof(argv[2]);
        }else if(is_assign==1){
            *(float*)_var->body = *(float*)_var_second->body;
        }
        logInfo("%s is %f\r\n", _var->name, *(float*)_var->body);

        break;
    case _lf:
        if(argc == 3&& is_assign==0){
            *(double*)_var->body = atof(argv[2]);
        }else if(is_assign==1){
            *(double*)_var->body = *(double*)_var_second->body;
        }
        logInfo("%s is %lf\r\n", _var->name, *(double*)_var->body);

        break;
    default:
        break;
    }
}
SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_MAIN), var, var, see or change variable);

void var_arr_register(void *var,const char* name, var_type_e type,int size)
{
    char name1[20]={0};
    switch (type)
        {

        case _8:
            for(int i=0;i<size;++i){
                sprintf(name1,"%s[%d]",name,i);
                var_register(&((char*)var)[i],name1,type);
            }
            break;
			
			
        //case _8:
        //    logInfo("%s is %d", _var->name, *(unsigned char*)_var->body);

        //    break;
        case _16:
            for(int i=0;i<size;++i){
                sprintf(name1,"%s[%d]",name,i);
                var_register(&((short*)var)[i],name1,type);
            }
            break;
        //case _u16:
        //    logInfo("%s is %d", _var->name, *( unsigned short*)_var->body);

        //    break;
        case _32:
            for(int i=0;i<size;++i){
                sprintf(name1,"%s[%d]",name,i);
                var_register(&((int*)var)[i],name1,type);
            }
            break;
        //case _u32:
        //    logInfo("%s is %d", _var->name, *(unsigned int*)_var->body);
        //    break;
        case _64:
            for(int i=0;i<size;++i){
                sprintf(name1,"%s[%d]",name,i);
                var_register(&((long long*)var)[i],name1,type);
            }
            break;
        //case _u64:
        //    logInfo("%s is %d", _var->name, *(unsigned long long*)_var->body);

        //    break;
        case _f:
            for(int i=0;i<size;++i){
                sprintf(name1,"%s[%d]",name,i);
                var_register(&((float*)var)[i],name1,type);
            }
            break;
        case _lf:
            for(int i=0;i<size;++i){
                sprintf(name1,"%s[%d]",name,i);
                var_register(&((double*)var)[i],name1,type);
            }
            break;
        default:
            break;
        }



}
void var_register(void* var,const char* name, var_type_e type)
{
    var_list_t* p = (var_list_t*)malloc(sizeof(var_list_t));

    if (p == NULL)return;
    var_list_t* p2;
    if (var != NULL)p->body = var;
    else return;
    if (name != NULL)
    {
        int len=strlen(name);
        char* pname = (char*)malloc(sizeof(char)*len+1);
        strcpy(pname,name);
        p->name = pname;
    }
    else return;
    p->type = type;
    p->next = 0;
    if (!_var_begin) {//
        _var_begin = p;
        return;
    }
    p2 = _var_begin;
    while ((p2->next)!=0)
    {
        p2 = p2->next;
    }//�ҵ����һ������
    p2->next = p;//��ӱ�������������
}





int db (int argc, char** argv)
{
//     extern struct fdb_kvdb kvdb;
//	if(argc==1)//���������1 �������е�������
//    {
//       fdb_kv_print(&kvdb);//��
//       return 0;
// 
//    }
//	if(argc==2)//���ֻ�������Ͳ��ң����û�о�ֱ�Ӵ�����Ĭ�ϸ�ֵΪ0
//	{
//		//�ڶ��������Ǽ�����һ�������Ǿ����������������ֵ�������Ǵ���������һ���Ǿ�����ڶ�����ֵ��������ֵ�Ĵ�С��
//		//fdb_kv_get_blob(&kvdb, argv[1] ,fdb_blob_make(&blob, &tea, sizeof(tea)));
//		DB_Set_API(argc,argv);
//		DB_Get_API(argc,argv);
//		 return 0;
//	}
//	if(argc==3)
//	{
		
		if(0==strcasecmp("0",argv[2]))
				 strcpy("null",argv[2]);
		DB_Co_API( argc, argv[1],argv[2]);
		
		
		
		//�ڶ��������Ǽ�����һ�������Ǿ����������������ֵ�������Ǵ���������һ���Ǿ�����ڶ�����ֵ��������ֵ�Ĵ�С��
		//fdb_kv_get_blob(&kvdb, argv[1] ,fdb_blob_make(&blob, &tea, sizeof(tea)));
		return 0;
    
//	}
	
}SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_MAIN), db, db,flash db test tea);
int DB_Set_API(int argc, char** argv)
{
	 extern struct fdb_kvdb kvdb;
	if(argc==1)
	{
       fdb_kv_print(&kvdb);//��
	}
	 float num=0.0;
	 struct fdb_blob blob;
	 fdb_kv_get_blob(&kvdb, argv[1] ,fdb_blob_make(&blob, &num, sizeof(num)));
			if (blob.saved.len > 0) 
			{
            printf("get the  value is %f\r\n", num);
      } 
			else
			{
            printf("get the  failed\r\n");
      }
	 printf("FDB get success\r\n");
			return 0;
}
int DB_Get_API(int argc, char** argv)
{
	 extern struct fdb_kvdb kvdb;
	if(argc==1)
	{
       fdb_kv_print(&kvdb);//��
	}
	 float num=0.0;
	 struct fdb_blob blob;
	 fdb_kv_set_blob(&kvdb, argv[1] ,fdb_blob_make(&blob, &num, sizeof(num)));
	 printf("FDB set success\r\n");
			return 0;
	
}
int DB_Co_API(int argc,char* name, char *KV)//name�� KV ֵ
{
			float num=0.0;
			struct fdb_blob blob;
			extern struct fdb_kvdb kvdb;
			if(argc==1)
			{
       fdb_kv_print(&kvdb);//��
			}
			else if(0==strcmp("del",name))
			{
					fdb_kv_del(&kvdb,KV);
					printf("Del the  value success");
			}
			else if(0==strcmp("null",KV))
			{
					num=0.0;
					fdb_kv_get_blob(&kvdb, name ,fdb_blob_make(&blob, &num, sizeof(num)));		
					if (blob.saved.len > 0) 
					{printf("Get the  value is %f\r\n", num);} 
					else
					{printf("The value does not exist \r\n");}
//					fdb_kv_set_blob(&kvdb, name ,fdb_blob_make(&blob, &num, sizeof(num)));
//					printf("Creat the  value is %f\r\n", num);
			}
			
			else
			{
				num=0.0;
				fdb_kv_get_blob(&kvdb, name ,fdb_blob_make(&blob, &num, sizeof(num)));
					if (blob.saved.len > 0) 
					{printf("Get the  value is %f\r\n", num);} 
					else
					{	printf("The value does not exist, but has been created \r\n");}
				char *endptr;
				num = strtod(KV, &endptr);
		//		printf("num=%f\r\n",num);
				fdb_kv_set_blob(&kvdb, name, fdb_blob_make(&blob, &num, sizeof(num)));
				printf("  value  change is %f\r\n", num);
			}
			return 0;
}
