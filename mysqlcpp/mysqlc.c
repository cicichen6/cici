#include <stdio.h>
#include <stdlib.h>
#include <mysql/mysql.h>
#include <string.h>

void connectDatabase(MYSQL*);
void checkIfDone(MYSQL*, int);
void addTableData(MYSQL*, char[], char[], int);
void deleTableData(MYSQL*, char[], int);
void reviseTableData(MYSQL*, char[], char[], int, int);
void showAll(MYSQL*);
void freeConnect(MYSQL*);


int main(){
    MYSQL *conn = NULL;
    char sql[500] = {0};
    connectDatabase(conn);
    //showAll(conn);
    freeConnect(conn);
    return 0;
}

void connectDatabase(MYSQL* conn){
    //初始化mysql
    mysql_init(conn);
    //conn = mysql_init(NULL);
    //mysql_library_init(0, NULL, NULL);
    //连接mysql
    //mysql_real_connect(&conn, "127.0.0.1", "root", "123456", "test01", 3306, NULL, 0);
    if(!(mysql_real_connect(conn, "127.0.0.1", "root", "123456", "test01", 0, NULL, 0))){
        printf( "Error connecting to database:%s\n",mysql_error(conn));
    }
    else{
        printf("Conn...");
    }
}

void checkIfDone(MYSQL* conn, int ret){
    printf("执行语句后，返回结果：%d\n", ret);	
    if(ret){ 
        printf("操作失败！失败原因：%s\n", mysql_error(conn));
    }
    else{
        printf("操作成功！\n");
    }
    printf("%ld row updated", mysql_affected_rows(conn));
}

void addTableData(MYSQL* conn, char sql[], char name[], int birth){
    snprintf(sql, 100, "INSERT INTO test01 VALUES('%s', %d);", name, birth);
    printf("插入sql语句：%s\n", sql);
    int ret = mysql_query(conn, sql);//执行，成功返回0
    checkIfDone(conn, ret);
}

void deleTableData(MYSQL* conn, char sql[], int id){
    sprintf(sql, "DELETE FROM test01 WHERE id = %d;", id);
    printf("插入sql语句：%s\n", sql);
    int ret = mysql_query(conn, sql);//执行，成功返回0
    checkIfDone(conn, ret);
}

void reviseTableData(MYSQL* conn, char sql[], char name[], int birth, int id){
    snprintf(sql, 100, "UPDATE test01 SET name = '%s', birth = %d WHERE id = %d;", name, birth, id);
    printf("插入sql语句：%s\n", sql);
    int ret = mysql_query(conn, sql);//执行，成功返回0
    checkIfDone(conn, ret);
} 

void showAll(MYSQL* conn){
    mysql_query(conn, "SELECT * FROM test01");
    MYSQL_RES *result = mysql_store_result(conn);
    unsigned int num_fields = mysql_num_fields(result);//返回多少列
    MYSQL_FIELD *field = mysql_fetch_field(result);
    while ((field = mysql_fetch_field(result))){//打印表头
	printf("%-20s", field->name);
    }
    MYSQL_ROW row = mysql_fetch_row(result);//读取每一行的数据
    while((row = mysql_fetch_row(result)) != NULL){
	for(int i = 0; i < num_fields; i++){
	    printf("%-20s", row[i]);
	}
    }
    free(field);
    mysql_free_result(result);//释放结果集；
}

void freeConnect(MYSQL* conn){
    //mysql_free_result(result);
   // mysql_library_end();
    mysql_close(conn);
}
