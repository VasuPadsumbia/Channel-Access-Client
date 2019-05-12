#include <stdio.h>
#include <string.h>
#include <cadef.h>
#define epicsAlarmGLOBAL
#include <alarm.h>
#include <epicsTime.h>

dbr_enum_t a;
dbr_enum_t store;
dbr_double_t c;
dbr_double_t data_1;
dbr_double_t data_2;
dbr_double_t data_3;
FILE* fp;
char* gapName_1="L4:TEMP1";
char* gapName_2="L4:TEMP2";
char* gapName_3="L4:TEMP3";
char* gapName_4="L4:TEMP4";
dbr_double_t temp_1;
dbr_double_t temp_2;
dbr_double_t temp_3;
dbr_double_t temp_4;
static char buffer [40];
epicsTimeStamp current;

void monitor(struct event_handler_args args)
{

    epicsTimeGetCurrent(&current);
    epicsTimeToStrftime(buffer, sizeof(buffer), "%d/%m/%y  %H:%M:%S.%04f", &current);
    if (args.status != ECA_NORMAL)
    {
        /* Something went wrong. */
        SEVCHK(args.status, "monitor");
        return;
    }
    /* Let's have a look at the type of the data.
       It should be one of the types that we have requested.
    */
   
    switch (args.type)
    {
        case DBR_STS_DOUBLE:
            {
                const struct dbr_sts_double* data = args.dbr;
                printf ("%s = %#g  %s\n", ca_name(args.chid), data->value, epicsAlarmSeverityStrings[data->severity]);
                if(strcmp(ca_name(args.chid),gapName_1) == 0)
                	temp_1 = data->value;
                else if(strcmp(ca_name(args.chid),"para_1") == 0)
                	data_1 = data->value;
                else if (strcmp(ca_name(args.chid),"para_2") == 0)
                	data_2 = data->value;
                else if (strcmp(ca_name(args.chid),gapName_2) == 0)
                  temp_2 = data->value;
                else if (strcmp(ca_name(args.chid),gapName_3) == 0)
                  temp_3 = data->value;
                else if (strcmp(ca_name(args.chid),gapName_4) == 0)
                  temp_4 = data->value;
                else
                	data_3 = data->value;
                break;
            }
        case DBR_STS_ENUM:
            {
                const struct dbr_sts_enum* data = args.dbr;
                printf ("%s = %i  %s\n", ca_name(args.chid), data->value, epicsAlarmSeverityStrings[data->severity]);
                if (strcmp(ca_name(args.chid),"start") == 0)
                	a = data->value;
                else
                	store = data->value;
                break;
            }
        default:
            printf ("%s unsupported data type\n", ca_name(args.chid));

    }
    printf("Temperature_1  %#g\n", temp_1);
    printf("Temperature_2  %#g\n", temp_2);
    printf("Temperature_3  %#g\n", temp_3);
    printf("Temperature_4  %#g\n", temp_4);
    printf("data_1  %#g\n", data_1);
    printf("data_2  %#g\n", data_2);
    printf("data_3  %#g\n", data_3);

     if(a == 1)
     {
    	 fp = fopen ("/home/vasu/Desktop/temp_data.csv","a");
    	 fprintf(fp, "%#g ,%#g , %#g, %#g, %s\n", temp_1, temp_2, temp_3, temp_4, buffer);
       printf("data logging into file temp_data.csv\n");
     }
     else if (store == 1)
     {
     	fp = fopen("/home/vasu/Desktop/para.csv", "a");
     	fprintf(fp, "%#g,%#g,%#g,%s\n",data_1, data_2, data_3, buffer);
      printf("parameters logging into file para.csv\n");
     }
     else if (a == 1 && store == 1){printf("data and parameters both logging\n");}
     else
      {
        printf("parameters and data logging off \n");
        return;
      }

    fclose(fp);


}



int main(void)
{
	fp = fopen ("/home/vasu/Desktop/temp_data.csv","a");
	fprintf(fp, "Temp_1, Temp_2, Temp_3, Temp_4, time\n");
	fclose(fp);
	fp = fopen ("/home/vasu/Desktop/para.csv","a");
	fprintf(fp, "para_1, para_2, para_3, time\n");
	fclose(fp);
    char* doneName="start";
    chid Temperature_1, Temperature_2,Temperature_3,Temperature_4, doneChannel, parameter_1, parameter_2, parameter_3, record;
    double search_timeout = 5.0; /* seconds */
    
    /* Step1: initialize channel access and search for all channels. */
    ca_context_create(ca_disable_preemptive_callback);
    
    /* ca_create_channel has more parameters than the old ca_search
       but we don't need them here.
    */
    ca_create_channel(gapName_1, NULL, NULL, CA_PRIORITY_DEFAULT, &Temperature_1);
    ca_create_channel(gapName_2, NULL, NULL, CA_PRIORITY_DEFAULT, &Temperature_2);
    ca_create_channel(gapName_3, NULL, NULL, CA_PRIORITY_DEFAULT, &Temperature_3);
    ca_create_channel(gapName_4, NULL, NULL, CA_PRIORITY_DEFAULT, &Temperature_4);
    ca_create_channel(doneName, NULL, NULL, CA_PRIORITY_DEFAULT, &doneChannel);
    ca_create_channel("para_1", NULL, NULL, CA_PRIORITY_DEFAULT, &parameter_1);
    ca_create_channel("para_2", NULL, NULL, CA_PRIORITY_DEFAULT, &parameter_2);
    ca_create_channel("para_3", NULL, NULL, CA_PRIORITY_DEFAULT, &parameter_3);
    ca_create_channel("record_para", NULL, NULL, CA_PRIORITY_DEFAULT, &record);
    SEVCHK(ca_pend_io(search_timeout), "ca_search");

    /* Step 2: setup the monitors */
    
    /* Create two monitors with different data types.
       Connect them to the same callback function.
       The 6th argument will be passed to the 'usr' element
       in the handler arguments. We don't need it here.
    */    
    ca_create_subscription(DBR_STS_DOUBLE, 1, Temperature_1, DBE_VALUE|DBE_ALARM, monitor, NULL, NULL);
    ca_create_subscription(DBR_STS_DOUBLE, 1, Temperature_2, DBE_VALUE|DBE_ALARM, monitor, NULL, NULL);
    ca_create_subscription(DBR_STS_DOUBLE, 1, Temperature_3, DBE_VALUE|DBE_ALARM, monitor, NULL, NULL);
    ca_create_subscription(DBR_STS_DOUBLE, 1, Temperature_4, DBE_VALUE|DBE_ALARM, monitor, NULL, NULL);
    ca_create_subscription(DBR_STS_ENUM, 1, doneChannel, DBE_VALUE|DBE_ALARM, monitor, NULL, NULL);
    ca_create_subscription(DBR_STS_DOUBLE, 1, parameter_1, DBE_VALUE|DBE_ALARM, monitor, NULL, NULL);
    ca_create_subscription(DBR_STS_DOUBLE, 1, parameter_2, DBE_VALUE|DBE_ALARM, monitor, NULL, NULL);
    ca_create_subscription(DBR_STS_DOUBLE, 1, parameter_3, DBE_VALUE|DBE_ALARM, monitor, NULL, NULL);
    ca_create_subscription(DBR_STS_ENUM, 1, record, DBE_VALUE|DBE_ALARM, monitor, NULL, NULL);
    /* In 3.13 we have actually used a macro with default
       values for some arguments. Here, we have to specify:
          * we want scalars, not arrays (count=1)
          * we are interested in value and alarm changes
    */
    
    SEVCHK(ca_flush_io(), "ca_add_event");    
    /* We have used ca_flush_io() here because there is nothing
       to wait for. We just send out the request.
       Note: ca_pend_io(timeout) works like ca_flush_io() plus
       additional waiting for outstanding replies.
    */
    
    /* Step 3: wait forever and do Channel Access in the background */
    ca_pend_event(0.0);

    /* We should never reach this point! */
    printf("Done\n");
    ca_context_destroy();
    return 0;  
}
