#include <my_header.h>
#include <mysql/mysql.h>

int main(int argc, char *argv[]){                                  
    
    //初始化
    MYSQL *pconn = mysql_init(NULL);
    if(pconn == NULL){
        printf("初始化失败: %s\n", mysql_error(pconn));
        return -1;
    }
    
    //建立连接
    if(mysql_real_connect(pconn, "localhost", "root", "123456", "test", 0, NULL, 0) == NULL){
        printf("连接数据库失败: %s\n", mysql_error(pconn));
    }
    
    //对数据库表中的数据进行: 增加、修改、删除、查找
    int ret = mysql_query(pconn, "select * from S");
    if(ret != 0){
        printf("查询失败: %s\n", mysql_error(pconn));
        return -1;
    }

    //获取查询结果
    MYSQL_RES *pres = mysql_store_result(pconn);
    if(pres == NULL){
        printf("查询结果失败: %s\n", mysql_error(pconn));
        return -1;
    }
    
    MYSQL_ROW row;
    //获取行
    while((row = mysql_fetch_row(pres))){
        //得到列数,遍历每一行的列
        for(int idx=0; idx<mysql_num_fields(pres); idx++){
            //对于每一行而言,打印这一行的每一列
            printf("%s\t\t", row[idx]);
        }
        printf("\n");
    }

    //释放资源
    mysql_free_result(pres);
    mysql_close(pconn);

    return 0;
}

