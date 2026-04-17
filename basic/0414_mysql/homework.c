#include <my_header.h>
#include <mysql/mysql.h>

int main(int argc, char *argv[]){                                  
    
    //初始化
    MYSQL *pconn = mysql_init(NULL);
    if (pconn == NULL) {
        fprintf(stderr, "初始化失败\n");
        return -1;
    }

    //连接数据库(参数：连接句柄, 地址, 用户, 密码, 库名, 端口, socket, 标志)
    if(mysql_real_connect(pconn, "localhost", "root", "123456", "test_homework", 0, NULL, 0) == NULL){
        fprintf(stderr, "连接数据库失败: %s\n", mysql_error(pconn));
        mysql_close(pconn);
        return -1;
    }
    printf("成功连接到数据库！\n\n");

    //插入数据
    const char *names[] = {"张三", "李四", "王五"};
    for(int i=0; i<3; i++){
        char sql[128];
        sprintf(sql, "insert into users (name, age) values ('%s', %d)", names[i], 20+i);
        int ret = mysql_query(pconn, sql);
        if(ret != 0){
            fprintf(stderr, "插入失败: %s\n", mysql_error(pconn));
        }
    }
    printf("插入数据完成。\n");


    //修改数据
    int ret = mysql_query(pconn, "update users set age=30 where name='张三'");
    if(ret != 0){
        fprintf(stderr, "修改失败: %s\n", mysql_error(pconn));
    }
    printf("修改数据完成（张三的年龄改为30）。\n");

    
    //删除数据
    ret = mysql_query(pconn, "delete from users where name='李四'");
    if(ret != 0){
        fprintf(stderr, "删除失败: %s\n", mysql_error(pconn));
    }
    printf("删除数据完成（李四已移除）。\n");

    
    //最终查询并打印结果
    //对数据库表中的数据进行查找并打印
    printf("最终表的数据如下：\n");
    ret = mysql_query(pconn, "select * from users");
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
    mysql_close(pconn);

    return 0;
}

