#include "db.hpp"

int main()
{
    MYSQL *mysql = blog_system::MysqlInit();
    if (mysql == NULL)
    {
        printf("mysql init error\n");
        return 0;
    }
    // blog_system::table_tag t_tag(mysql); 
    // Json::Value tag;
    // tag["tag_name"] = "C++";
    // t_tag.Insert(tag);

    // tag.clear();
    // tag["tag_name"] = "Java";
    // t_tag.Insert(tag);
    
    //blog_system::table_user t_user(mysql);
    //Json::Value user;
    //user["user_id"] = 100;
    //user["user_name"] = "张三2";
    // t_user.Insert(user);
    //binuser.clear();
    //user["user_name"] = "张三2";
    //t_user.UpDate(1, user);
    //t_user.Delete(1);
    //Json::Reader reader;

    //Json::StyledWriter writer;
    //t_user.GetOne(1, user);
    
    //std::cout << writer.write(user) << std::endl;
    
    blog_system::table_blog tableUser(mysql);
    Json::Value val;
  
    tableUser.GetUser(2, val);
    
    Json::StyledWriter writer;
    std::cout << writer.write(val) << std::endl;

    blog_system::MysqlClose(mysql);
    return 0;
}
