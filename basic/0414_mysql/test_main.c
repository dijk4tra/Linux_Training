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
    
    char *temp[10] = {"张三", "李四", "王五", "赵六", "孙七",
                      "安妮", "空灵", "哩哩", "景天", "花生"};

    //对数据库表进行插入操作
    for(int idx=1; idx<11; ++idx){
        char arr[100] = {0};
        sprintf(arr, "insert into person values (%d, '%s', %d)",
                idx, temp[idx-1], idx+10);
        int ret = mysql_query(pconn, arr);
        if(ret != 0)
        {
            printf("插入失败: %s\n", mysql_error(pconn));
            return -1;
        }
    }

    //对数据库表中的数据进行查找并打印
   int ret = mysql_query(pconn, "select * from person");
    if(ret != 0){
        printf("查询失败: %s\n", mysql_error(pconn));
        return -1;
    }

    //获取查询的结果
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

