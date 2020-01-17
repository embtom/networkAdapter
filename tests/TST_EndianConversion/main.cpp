#include <gtest/gtest.h>
#include <iostream>
#include "Meta.h"


#define GTEST_BOX                   "[     cout ] "

struct Person
{
   std::string        name;
   unsigned int                age;
};


namespace meta
{


template <>
inline auto registerMembers<Person>()
{
   return members(
      member("name", &Person::name),
      member("age", &Person::age)
   );
}
}




TEST(metaStuff,test)
{

}

int main(int argc, char **argv)
{
   //  testing::InitGoogleTest(&argc, argv);
   //  return RUN_ALL_TESTS();

       Person person({"Dodo", 39});

   int a = meta::getMemberCount<Person>();
   std::string b = meta::getName<Person>();
   std::string name = meta::getMemberValue<std::string, Person>(person, "name");
   auto mem = meta::getMembers<Person>();
    
   std::cout << "count" << a << b << std::endl;

   meta::doForAllMembers<Person>(
        [&](const auto& member)
        {
           using MemberT = meta::get_member_type<decltype(member)>;
           MemberT td = member.get(person);
           std::cout << typeid(MemberT).name() << std::endl;
           std::cout << member.getName() << " : " << td << std::endl;
           //std::cout << member << std::endl;
        });
}  