#include "db.hpp"
#include "httplib.h"

#define WWWROOT "../www"

blog_system::table_tag *table_tag;
blog_system::table_user *table_user;
blog_system::table_blog *table_blog;

void InsertUser(const httplib::Request &req, httplib::Response &res)
{
    // 解析Json字符串为Json对象
    Json::Value val;
    Json::Reader reader;
    bool ret = reader.parse(req.body, val);
    if (ret == false)
    {
        printf("insert user failed: read req.body false\n");
        res.status = 400; // 请求错误
        return ;
    }
    // 调用数据库插入接口
    ret = table_user->Insert(val);
    if (ret == false)
    {
        printf("insert user failed: insert database false\n");
        res.status = 500; // 服务器内部错误
        return ;
    }
    // 设置状态码，退出
    res.status = 200;
    return ;
}
void UpdateUser(const httplib::Request &req, httplib::Response &res)
{
    int user_id = std::stoi(req.matches[1]); // matches[0]保存的是整个字符串，matches[1]保存的是提取到的字符串
    Json::Reader reader;
    Json::Value val;
    bool ret = reader.parse(req.body, val);
    if (ret == false)
    {
        printf("Update user failed: read req.body false\n");
        res.status = 400;
        return ;
    }
    ret = table_user->UpDate(user_id, val);
    if (ret == false)
    {
        printf("Update user failed: update database false\n");
        res.status = 500;
        return ;
    }
    res.status = 200;
    return ;
}
void GetOneUser(const httplib::Request &req, httplib::Response &res)
{
    int user_id = 0;
    user_id = std::stoi(req.matches[1]);
    Json::Value val;
    bool ret = table_user->GetOne(user_id, val);
    if (ret == false)
    {
        printf("GetOne user failed: GetOne database false\n");        
        res.status = 500;
        return ;
    }
    Json::FastWriter writer;
    // 将Json对象转换为Json字符串
    res.body = writer.write(val);
    res.status = 200;
    return ;
}
void GetAllUser(const httplib::Request &req, httplib::Response &res)
{
    Json::Value val;
    bool ret = table_user->GetAll(val);
    if (ret == false)
    {
        printf("GetALL user failed: GetALL database false\n");        
        res.status = 500;
        return ;
    }
    Json::FastWriter writer;
    res.body = writer.write(val);
    res.status = 200;
    return ;
}
void DeleteUser(const httplib::Request &req, httplib::Response &res)
{
    int user_id = std::stoi(req.matches[1]);
    bool ret = table_user->Delete(user_id);
    if (ret == false)
    {
        printf("Delete user failed: Delete database false\n");
        res.status = 500;
    }
    res.status = 200;
    return ;
}

void InsertTag(const httplib::Request &req, httplib::Response &res)
{
    Json::Reader reader ;
    Json::Value val;
    bool ret = reader.parse(req.body, val);
    if (ret == false)
    {
        printf("insert tag failed: read req.body false\n");
        res.status = 400;
        return ;
    }
    ret = table_tag->Insert(val);
    if (ret == false)
    {
        printf("insert tag failed: insert database false\n");
        res.status = 500;
        return;
    }
    res.status = 200;
    return ;
}
void UpdateTag(const httplib::Request &req, httplib::Response &res)
{
    int tag_id = std::stoi(req.matches[1]);
    Json::Value val;
    Json::Reader reader;
    bool ret = reader.parse(req.body, val);
    if (ret == false)
    {
        printf("Update tag failed: read req.body false\n");
        res.status = 400;
        return ;
    }
    ret = table_tag->UpDate(tag_id, val);
    if (ret == false)
    {
        printf("UpDate tag failed: UpDate database false\n");
        res.status = 500;
        return ;
    }
    res.status = 200;
    return ;
}
void GetOneTag(const httplib::Request &req, httplib::Response &res)
{
    int tag_id = std::stoi(req.matches[1]);
    Json::Value val;
    bool ret = table_tag->GetOne(tag_id, val);
    if (ret == false)
    {
        printf("GetOne tag failed: GetOne database false\n");
        res.status = 500;
        return ;
    }
    Json::FastWriter writer;
    res.body = writer.write(val);
    res.status = 200;
    return ;
}
void GetAllTag(const httplib::Request &req, httplib::Response &res)
{
    Json::Value val;
    Json::FastWriter writer;
    bool ret = table_tag->GetAll(val);
    if (ret == false)
    {
        printf("GetAll tag failed: GetAll database false\n");
        res.status = 500;
        return ;
    }
    res.body = writer.write(val);
    res.status = 200;
    return ;
}
void DeleteTag(const httplib::Request &req, httplib::Response &res)
{
    int tag_id = std::stoi(req.matches[1]);
    bool ret = table_tag->Delete(tag_id);
    if (ret == false)
    {
        printf("Delete tag failed: Delete database false\n");
        res.status = 500;
        return ;
    }
    res.status = 200;
    return ;
}

void InsertBlog(const httplib::Request &req, httplib::Response &res)
{
    Json::Reader reader;
    Json::Value val;
    bool ret = reader.parse(req.body, val);
    if (ret == false)
    {
        printf("insert blog failed: read req.body false\n");
        res.status = 400;
        return ;
    }
    ret = table_blog->Insert(val);
    if (ret == false)
    {
        printf("insert blog failed: insert database false\n");
        res.status = 504;
        return;
    }
    res.status = 200;
    return;
}
void UpdateBlog(const httplib::Request &req, httplib::Response &res)
{
    int blog_id = std::stoi(req.matches[1]);
    Json::Reader reader;
    Json::Value val;
    bool ret = reader.parse(req.body, val);
    if (ret == false)
    {
        printf("Update blog failed: read req.body false\n");
        res.status = 400;
        return ;
    }
    ret = table_blog->UpDate(blog_id, val);
    if (ret == false)
    {
        printf("Update blog failed: Update database false\n");
        res.status = 500;
        return;
    }
    res.status = 200;
    return ;
}
void GetOneBlog(const httplib::Request &req, httplib::Response &res)
{
    int blog_id = std::stoi(req.matches[1]);
    Json::Value val;
    bool ret = table_blog->GetOne(blog_id, val);
    if (ret == false)
    {
        printf("GetOne blog failed: GetOne database false\n");
        res.status = 500;
        return ;
    }
    Json::FastWriter writer;
    res.body = writer.write(val);
    res.status = 200;
    return ;
}
void GetAllBlog(const httplib::Request &req, httplib::Response &res)
{
    Json::Value val;
    if (req. has_param("tag_id"))
    {
        int tag_id = std::stoi(req.get_param_value("tag_id")) ;
        bool ret = table_blog->GetTag(tag_id, val);
        if (ret == false)
        {
            printf("Get tag blog failed: Get tag database false\n");
            res.status = 500;
            return;
          }
    }else if (req.has_param("user_id"))
    {
        int user_id = std::stoi(req.get_param_value("user_id")) ;
        bool ret = table_blog->GetUser(user_id, val);
        if (ret == false)
        {
            printf("Get user blog failed: Getuesr database false\n");
            res.status = 500;
            return;
        }
    }else
    {
        bool ret = table_blog->GetAll (val) ;
        if (ret == false) 
        {
            printf("GetAll blog failed: GetAll database false\n"); 
            res.status = 500;
            return;
        }
    }
    Json::FastWriter writer;
    res.body = writer.write(val);
    res.status = 200;
    return ;
}
void DeleteBlog(const httplib::Request &req, httplib::Response &res)
{
    int blog_id = std::stoi(req.matches[1]);
    bool ret = table_blog->Delete(blog_id);
    if (ret == false)
    {
        printf("Delete blog failed: Delete database false\n");
        res.status = 500;
        return ;
    }
    res.status = 200;
    return ;
}

int main()
{
    MYSQL *mysql = blog_system::MysqlInit();
    if (mysql == NULL)
    {
        return -1;
    }
    table_tag = new blog_system::table_tag(mysql);
    table_user = new blog_system::table_user(mysql);
    table_blog = new blog_system::table_blog(mysql);
    httplib::Server server;
    // 设置静态文件目录
    server.set_base_dir(WWWROOT);

    // Get 获取, Post 添加, Put 更新, Delete 删除
    
    // User
    server.Post(R"(/user)", InsertUser);
    server.Put(R"(/user/(\d+))", UpdateUser);
    server.Delete(R"(/user/(\d+))", DeleteUser);
    server.Get(R"(/user/(\d+))", GetOneUser);
    server.Get(R"(/user)", GetAllUser);

    // Tag
    server.Post(R"(/tag)", InsertTag);
    server.Put(R"(/tag/(\d+))", UpdateTag);
    server.Delete(R"(/tag/(\d+))", DeleteTag);
    server.Get(R"(/tag/(\d+))", GetOneTag);
    server.Get(R"(/tag)", GetAllTag);

    // Blog
    server.Post(R"(/blog)", InsertBlog);
    server.Put(R"(/blog/(\d+))", UpdateBlog);
    server.Delete(R"(/blog/(\d+))", DeleteBlog);
    server.Get(R"(/blog/(\d+))", GetOneBlog);
    server.Get(R"(/blog)", GetAllBlog);

    server.listen("0.0.0.0", 9000);
    return 0;
}
