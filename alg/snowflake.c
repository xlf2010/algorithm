/**
 * 雪花算法生成id实现
 * 64 bit位中
 * 从左往右为，第一位为符号位，2~42，共41位为毫秒数(当前时间戳毫秒数-配置的开始毫秒)，
 * 43~47共5位为数据中心id，
 * 48~52共5位为机器id，53~63共12位为毫秒后序列
 */
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include<sys/time.h>
#include<pthread.h>

//开始时间，毫秒
static long long start_timestamp;
//数据中心ID
static int data_center_id;
//机器ID
static int worker_id;
//数据中心位数大小
static int data_center_bit_size;
//workerid位数大小
static int worker_id_bit_size;
//毫秒后位数大小
static int seq_bit_size;
//时间戳移位大小
static int timestamp_shift;
//数据中心ID移位大小
static int data_center_shift;
//机器ID移位大小
static int worker_shift;
//每毫秒能产生的最大ID
static int max_id_per_mills_second;
//上次操作时间
static long long last_timestamp;

//上次执行的序列号
static int seq=0;

static pthread_mutex_t mtx=PTHREAD_MUTEX_INITIALIZER;

//获取当前毫秒数
static long long get_current_millis(){
	static struct timeval tv;
	gettimeofday(&tv,NULL);
	long long sec = (long long)tv.tv_sec * 1000;
	int mil_sec =  tv.tv_usec/1000;
	//printf("get now:tvsec=%ld,tvusec=%ld,sec=%lld,mil_sec=%ld,add=%lld\n",tv.tv_sec,tv.tv_usec,sec,mil_sec,sec+mil_sec);
	return mil_sec+sec;
}

static long long get_next_millis(long long ts){
	long long now=get_current_millis();
	while(now<=ts)
		now=get_current_millis();
	return now;
}

static long long get_next_id(){
	long long now=get_current_millis();
	pthread_mutex_lock(&mtx);
	if(now<last_timestamp){
		printf("last timestamp error,now is : %lld,last timestamp is : %lld",now,last_timestamp);
		return -1;
	}
	if(now==last_timestamp){
		seq=(seq+1)&max_id_per_mills_second;
		if(seq==0){
			now=get_next_millis(now);
			seq=0;
		}
	}else{
		seq=0;	
	}
	last_timestamp=now;
	pthread_mutex_unlock(&mtx);
	long long res= ((last_timestamp-start_timestamp)<<timestamp_shift)|(data_center_id<<data_center_shift)
		|(worker_id<<worker_shift)|seq;
	//printf("get id res=%lld,lt=%lld,st=%lld,tims=%lld\n",res,last_timestamp,start_timestamp,((last_timestamp-start_timestamp)<<timestamp_shift));
	return res;
}

void num_to_binary(long long source,char *ret,int bit_len){
	int i;
	for(i=0;i<bit_len;i++){
		*(ret+(bit_len-i-1))=(int)((source>>i)&1)+'0';
	}
}

int main(int argc,char** argv){
	start_timestamp=1<<10;
		//get_current_millis();
	data_center_bit_size=5;
	worker_id_bit_size=5;
	seq_bit_size=12;
	data_center_id=1;
	worker_id=1;
	timestamp_shift=data_center_bit_size+worker_id_bit_size+seq_bit_size;
	data_center_shift=worker_id_bit_size+seq_bit_size;
	worker_shift=seq_bit_size;
	max_id_per_mills_second=-1L^(-1L<<seq_bit_size);
	
	int i,num=100;
	long long a[num];
	for(i=0;i<num;i++){
		long long id=get_next_id();
		a[i]=id;
	}
	int j,bit_len=sizeof(long long)*8;
	char *ret =(char *)malloc(sizeof(char)*bit_len+1);
	*(ret+bit_len)='\0';
	for(i=0;i<num;i++){
		num_to_binary(a[i],ret,bit_len);
		printf("value=%lld,ret len=%d,binary= %s \n",a[i],strlen(ret),ret);
	}
	free(ret);
	return 0;
}
