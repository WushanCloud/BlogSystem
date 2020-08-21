#include <iostream>
#include <stdio.h>
#include <string>
#include <mysql/mysql.h>
#include <jsoncpp/json/json.h>

#define MYSQL_HOST  "127.0.0.1"
#define MYSQL_USER  "root"
#define MYSQL_PASS  "111111"
#define MYSQL_DB    "blog_system"

namespace blog_system{
    // 初始化mysql句柄
    static MYSQL* MysqlInit();
    // 关闭mysql句柄
    static bool MysqlClose(MYSQL *mysql);
    // 执行mysql语句
    static bool MysqlQuery(MYSQL *mysql, const std::string& sql);

    static MYSQL* MysqlInit()
    {
        MYSQL *mysql = NULL;
        // 初始化mysql句柄
        mysql = mysql_init(NULL);
        if (mysql == NULL)
        {
            printf("mysql init error : %s\n", mysql_error(mysql));
            return NULL;
        }
        // 选择mysql服务器
        if (!mysql_real_connect(mysql, MYSQL_HOST, MYSQL_USER, MYSQL_PASS, MYSQL_DB, 0, NULL, 0))
        {
            printf("mysql connect error : %s\n", mysql_error(mysql));
            mysql_close(mysql);
            return NULL;
        }
        // 设置连接的数据库
        if (mysql_select_db(mysql, MYSQL_DB))
        {
            printf("mysql database select error : %s\n", mysql_error(mysql));
            mysql_close(mysql);
            return NULL;
        }
        // 设置字符编码集
        if (mysql_set_character_set(mysql, "utf8"))
        {
            printf("set character error : %s\n", mysql_error(mysql));
            mysql_close(mysql);
            return NULL;
        }
        
        return mysql;
    }
    
    static bool MysqlClose(MYSQL *mysql)
    {
        if (mysql)
        {
            mysql_close(mysql);
        }
        return true;
    }

    static bool MysqlQuery(MYSQL *mysql, const std::string& sql)
    {
        if (mysql_query(mysql, sql.c_str()))
        {
            printf("sql : %s  false : %s\n", sql.c_str(), mysql_error(mysql));
            return false;
        }
        return true;
    }

    // 数据库——博标签类
    class table_tag
    {
    public:
        table_tag(MYSQL *mysql)
            :_mysql(mysql)
        {}
        bool Insert(const Json::Value &tag);
        bool Delete(int tag_id);
        bool UpDate(int tag_id, const Json::Value &tag);
        bool GetAll(Json::Value &tags);
        bool GetOne(int tag_id, Json::Value &tag);
    private:
        MYSQL *_mysql;
    };
    
    bool table_tag::Insert(const Json::Value &tag)
    {
#define INSERT_TAG "insert into table_tag values (NULL, '%s');"
        char buf[4096] = {0};
        sprintf(buf, INSERT_TAG, tag["tag_name"].asCString());

        if (!MysqlQuery(_mysql, buf))
        {
            return false;
        }
        return true;
    }
    
    bool table_tag::Delete(int tag_id)
    {
#define DELETE_TAG "delete from table_tag where tag_id = %d;"
        char buf[4096] = {0};
        sprintf(buf, DELETE_TAG,tag_id);
        
        if (!MysqlQuery(_mysql, buf))
        {
            return false;
        }
        return true;
    }
        
    bool table_tag::UpDate(int tag_id, const Json::Value &tag)
    {
#define UPDATE_TAG "update table_tag set tag_name = '%s' where tag_id = %d;"
        char buf[4096] = {0};
        sprintf(buf, UPDATE_TAG, tag["tag_name"].asCString(), tag_id);
        
        if (!MysqlQuery(_mysql, buf))
        {
            return false;
        }
        return true;
    }
    
    bool table_tag::GetAll(Json::Value &tags)
    {
#define GETALL_TAG "select * from table_tag;"
        if (!MysqlQuery(_mysql, GETALL_TAG))
        {
            return false;
        }
        // 获取结果集
        MYSQL_RES * res = mysql_store_result(_mysql);
        if (res == NULL)
        {
            printf("Get results fail : %s\n", mysql_error(_mysql));
            return false;
        }
        // 获取结果集行数
        int res_rows = mysql_num_rows(res);
        if (res_rows <= 0)
        {
            printf ("Get all tags result is NULL\n");
            mysql_free_result(res);
            return false;
        }
        // 返回结果集
        MYSQL_ROW row;
        Json::Value tag;
        for (int i = 0; i < res_rows; i++)
        {
            row = mysql_fetch_row(res);
            tag["tag_id"] = std::stoi(row[0]);
            tag["tag_name"] = row[1];
            tags.append(tag);
        }
        mysql_free_result(res);
        return true;
    }

    bool table_tag::GetOne(int tag_id, Json::Value &tag)
    {
#define GETONE_TAG "select tag_name from table_tag where tag_id = %d;"
        char buf[4096] = {0};
        sprintf(buf, GETONE_TAG, tag_id);
        
        if (!MysqlQuery(_mysql, buf))
        {
            return false;
        }
        // 获取结果集
        MYSQL_RES * res = mysql_store_result(_mysql);
        if (res == NULL)
        {
            printf("Get results fail : %s\n", mysql_error(_mysql));
            return false;
        }
        // 获取结果集行数
        int res_rows = mysql_num_rows(res);
        if (res_rows != 1)
        {
            printf ("Get one tags result is NULL\n");
            mysql_free_result(res);
            return false;
        }
        // 返回结果集
        MYSQL_ROW row;
        row = mysql_fetch_row(res);
        tag["tag_id"] = tag_id;
        tag["tag_name"] = row[0];
        mysql_free_result(res);
        return true;
    }

    // 数据库—博客用户类
    class table_user
    {
    public:
        table_user(MYSQL *mysql)
            :_mysql(mysql)
        {}
        bool Insert(const Json::Value &user);
        bool Delete(int user_id);
        bool UpDate(int user_id, const Json::Value &user);
        bool GetAll(Json::Value &users);
        bool GetOne(int user_id, Json::Value &user);
    private:
        MYSQL *_mysql;
    };

    bool table_user::Insert(const Json::Value &user)
    {
#define INSERT_USER "insert into table_user values (NULL, '%s');"
        char buf[4096] = {0};
        sprintf(buf, INSERT_USER, user["user_name"].asCString());
        
        if (!MysqlQuery(_mysql, buf))
        {
            return false;
        }
        return true;
    }
    
    bool table_user::Delete(int user_id)
    {
#define DELETE_USER "delete from table_user where user_id = %d;"
        char buf[4096] = {0};
        sprintf(buf, DELETE_USER,user_id);
        
        if (!MysqlQuery(_mysql, buf))
        {
            return false;
        }
        return true;
    }
        
    bool table_user::UpDate(int user_id, const Json::Value &user)
    {
#define UPDATE_USER "update table_user set user_name = '%s' where user_id = %d;"
        char buf[4096] = {0};
        sprintf(buf, UPDATE_USER, user["user_name"].asCString(), user_id);
        
        if (!MysqlQuery(_mysql, buf))
        {
            return false;
        }
        return true;
    }
    
    bool table_user::GetAll(Json::Value &users)
    {
#define GETALL_USER "select * from table_user;"
        if (!MysqlQuery(_mysql, GETALL_USER))
        {
            return false;
        }
        // 获取结果集
        MYSQL_RES * res = mysql_store_result(_mysql);
        if (res == NULL)
        {
            printf("Get results fail : %s\n", mysql_error(_mysql));
            return false;
        }
        // 获取结果集行数
        int res_rows = mysql_num_rows(res);
        if (res_rows <= 0)
        {
            printf ("Get all users result is NULL\n");
            mysql_free_result(res);
            return false;
        }
        // 返回结果集
        MYSQL_ROW row;
        Json::Value user;
        for (int i = 0; i < res_rows; i++)
        {
            row = mysql_fetch_row(res);
            user["user_id"] = std::stoi(row[0]);
            user["user_name"] = row[1];
            users.append(user);
        }
        mysql_free_result(res);
        return true;
    }

    bool table_user::GetOne(int user_id, Json::Value &user)
    {
#define GETONE_USER "select user_name from table_user where user_id = %d;"
        char buf[4096] = {0};
        sprintf(buf, GETONE_USER, user_id);
        
        if (!MysqlQuery(_mysql, buf))
        {
            return false;
        }
        // 获取结果集
        MYSQL_RES * res = mysql_store_result(_mysql);
        if (res == NULL)
        {
            printf("Get results fail : %s\n", mysql_error(_mysql));
            return false;
        }
        // 获取结果集行数
        int res_rows = mysql_num_rows(res);
        if (res_rows != 1)
        {
            printf ("Get one users result is NULL\n");
            mysql_free_result(res);
            return false;
        }
        // 返回结果集
        MYSQL_ROW row;
        row = mysql_fetch_row(res);
        user["user_id"] = user_id;
        user["user_name"] = row[0];
        mysql_free_result(res);
        return true;
    }
    
    // 数据库—博客信息类
    class table_blog
    {
    public:
        table_blog(MYSQL *mysql)
            :_mysql(mysql)
        {}
        bool Insert(const Json::Value &blog);
        bool Delete(int blog_id);
        bool UpDate(int blog_id, const Json::Value &blog);
        bool GetAll(Json::Value &blogs);
        bool GetOne(int blog_id, Json::Value &blog);
        bool GetTag(int tag_id, Json::Value &blogs);
        bool GetUser(int user_id, Json::Value &blogs);
    private:
        MYSQL *_mysql;
    };
    
    bool table_blog::Insert(const Json::Value &blog)
    {
#define INSERT_BLOG "insert into table_blog values (NULL, %d, %d, '%s', '%s', now());"
        char buf[4096] = {0};
        sprintf(buf, INSERT_BLOG, blog["user_id"].asInt(), blog["tag_id"].asInt(), blog["title"].asCString(), blog["content"].asCString());
        
        if (!MysqlQuery(_mysql, buf))
        {
            return false;
        }
        return true;
    }
        
    bool table_blog::Delete(int blog_id)
    {
#define DELETE_BLOG "delete from table_blog where user_id = %d;"
        char buf[4096] = {0};
        sprintf(buf, DELETE_BLOG, blog_id);
        
        if (!MysqlQuery(_mysql, buf))
        {
            return false;
        }
        return true;
    }
    
    bool table_blog::UpDate(int blog_id, const Json::Value &blog)
    {
#define UPDATE_BLOG "update table_blog set user_id = %d, tag_id = %d, title = '%s', content = '%s' where blog_id = %d;"
        char buf[4096] = {0};
        sprintf(buf, UPDATE_BLOG, blog["user_id"].asInt(), blog["tag_id"].asInt(), blog["title"].asCString(), blog["content"].asCString(), blog_id);
        
        if (!MysqlQuery(_mysql, buf))
        {
            return false;
        }
        return true;
    }
        
    bool table_blog::GetAll(Json::Value &blogs)
    {
#define GETALL_BLOG "select * from table_blog;"
        if (!MysqlQuery(_mysql, GETALL_BLOG))
        {
            return false;
        }
        // 获取结果集
        MYSQL_RES * res = mysql_store_result(_mysql);
        if (res == NULL)
        {
            printf("Get results fail : %s\n", mysql_error(_mysql));
            return false;
        }
        // 获取结果集行数
        int res_rows = mysql_num_rows(res);
        if (res_rows <= 0)
        {
            printf ("Get all blogs result is NULL\n");
            mysql_free_result(res);
            return false;
        }
        // 返回结果集
        MYSQL_ROW row;
        Json::Value blog;
        for (int i = 0; i < res_rows; i++)
        {
            row = mysql_fetch_row(res);
            blog["blog_id"] = std::stoi(row[0]);
            blog["user_id"] = std::stoi(row[1]);
            blog["tag_id"] = std::stoi(row[2]);
            blog["title"] = row[3];
            blog["content"] = row[4];
            blog["ctime"] = row[5];
            blogs.append(blog);
        }
        return true;
    }
        
    bool table_blog::GetOne(int blog_id, Json::Value &blog)
    {
#define GETONE_BLOG "select * from table_blog where blog_id = %d;"
        char buf[4096] = {0};
        sprintf(buf, GETONE_BLOG, blog_id);

        if (!MysqlQuery(_mysql, buf))
        {
            return false;
        }
        // 获取结果集
        MYSQL_RES * res = mysql_store_result(_mysql);
        if (res == NULL)
        {
            printf("Get results fail : %s\n", mysql_error(_mysql));
            return false;
        }
        // 获取结果集行数
        int res_rows = mysql_num_rows(res);
        if (res_rows != 1)
        {
            printf ("Get blog result is NULL\n");
            mysql_free_result(res);
            return false;
        }
        // 返回结果集
        MYSQL_ROW row;
        row = mysql_fetch_row(res);
        blog["blog_id"] = std::stoi(row[0]);
        blog["user_id"] = std::stoi(row[1]);
        blog["tag_id"] = std::stoi(row[2]);
        blog["title"] = row[3];
        blog["content"] = row[4];
        blog["ctime"] = row[5];
        return true;
    }
        
    bool table_blog::GetTag(int tag_id, Json::Value &blogs)
    {
#define GETTAG_BLOG "select * from table_blog where tag_id = %d;"
        char buf[4096] = {0};
        sprintf(buf, GETTAG_BLOG, tag_id);

        if (!MysqlQuery(_mysql, buf))
        {
            return false;
        }
        // 获取结果集
        MYSQL_RES * res = mysql_store_result(_mysql);
        if (res == NULL)
        {
            printf("Get results fail : %s\n", mysql_error(_mysql));
            return false;
        }
        // 获取结果集行数
        int res_rows = mysql_num_rows(res);
        if (res_rows <= 0)
        {
            printf ("Get all tag blogs result is NULL\n");
            mysql_free_result(res);
            return false;
        }
        // 返回结果集
        MYSQL_ROW row;
        Json::Value blog;
        for (int i = 0; i < res_rows; i++)
        {
            row = mysql_fetch_row(res);
            blog["blog_id"] = std::stoi(row[0]);
            blog["user_id"] = std::stoi(row[1]);
            blog["tag_id"] = std::stoi(row[2]);
            blog["title"] = row[3];
            blog["content"] = row[4];
            blog["ctime"] = row[5];
            blogs.append(blog);
        }
        return true;
    }   

    bool table_blog::GetUser(int user_id, Json::Value &blogs)
    {
#define GETUSER_BLOG "select * from table_blog where user_id = %d;"
        char buf[4096] = {0};
        sprintf(buf, GETUSER_BLOG, user_id);
        if (!MysqlQuery(_mysql, buf))
        {
            return false;
        }
        // 获取结果集
        MYSQL_RES * res = mysql_store_result(_mysql);
        if (res == NULL)
        {
            printf("Get results fail : %s\n", mysql_error(_mysql));
            return false;
        }
        // 获取结果集行数
        int res_rows = mysql_num_rows(res);
        if (res_rows <= 0)
        {
            printf ("Get all user blogs result is NULL\n");
            mysql_free_result(res);
            return false;
        }
        // 返回结果集
        MYSQL_ROW row;
        Json::Value blog;
        for (int i = 0; i < res_rows; i++)
        {
            row = mysql_fetch_row(res);
            blog["blog_id"] = std::stoi(row[0]);
            blog["user_id"] = std::stoi(row[1]);
            blog["tag_id"] = std::stoi(row[2]);
            blog["title"] = row[3];
            blog["content"] = row[4];
            blog["ctime"] = row[5];
            blogs.append(blog);
        }
        return true;
    }

}
