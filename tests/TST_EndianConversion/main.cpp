#include <gtest/gtest.h>
#include <iostream>
#include <EndianConvert.h>

#include <EndianOrder.h>
#include <EndianMembers.h>
#include <EndianMeta.h>

#define GTEST_BOX                   "[     cout ] "



struct Person
{
   uint16_t       name;
   unsigned int                age;
   std::array<uint16_t,4>      arr;
   
};


namespace EtEndian
{

template <>
inline auto registerName<Person>()
{
   return "Person";
}

template <>
inline auto registerMembers<Person>()
{
   return members(
      member("name", &Person::name),
      member("age",  &Person::age),
      member("arr",  &Person::arr)
      
     /*  EtEndian::member("arr",  &Person::arr) */
   );
}
}

TEST(EndianMeta, EndianMembers)
{
   Person person({15, 39, {1,2,3,4}});
   {
      auto bla = EtEndian::member("arr",  &Person::age);
      std::cout << "VarName: " << bla.getName() << std::endl;
      using MemberT = EtEndian::get_member_type<decltype(bla)>;
      const MemberT test = bla.get(person);
      bla.set(person,120);
   }
   {
      auto bla = EtEndian::member("arr",  &Person::arr);
      std::cout << "VarName: " << bla.getName() << std::endl;
      using MemberT = EtEndian::get_member_type<decltype(bla)>;
      MemberT test = bla.get(person);
      MemberT test1 {1,2,3,4};
   }
   {
      auto bla = EtEndian::member("arr",  &Person::arr);
      using MemberT = EtEndian::get_member_type<decltype(bla)>;
      MemberT& test = bla.getRef(person);
      test[0] = 500;
   }
}

TEST(EndianMeta, EndianMeta)
{
   EXPECT_EQ(EtEndian::getMemberCount<Person>(), 3);
   EXPECT_EQ(EtEndian::getName<Person>(), "Person");
   EXPECT_EQ(EtEndian::isRegistered<Person>(), true);

   Person person({15, 39, {1,2,3,4}});

   EtEndian::doForAllMembers<Person>(
      [&person](const auto& member)
   {
      using MemberT = EtEndian::get_member_type<decltype(member)>;
      MemberT td = member.get(person);
      std::cout << GTEST_BOX << member.getName() << std::endl;
   });   
}






int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
   int ret =RUN_ALL_TESTS();

   Person person({15, 39, {1,2,3,4}});



   // int a = meta::getMemberCount<Person>();
   // std::string b = meta::getName<Person>();
   // std::string name = meta::getMemberValue<std::string, Person>(person, "name");
   // auto mem = meta::getMembers<Person>();

   // std::cout << "count" << a << b << std::endl;


   EtEndian::ConverterFunc<Person> serializeFunc(person);
   EtEndian::doForAllMembers<Person>(serializeFunc);



   EtEndian::CNetOrder conversion = person;
   auto t = conversion.NetworkOrder();

   std::cout << t.age << std::endl;;

   //EtNet::CNetOrder conversion2 = conversion;



   // meta::doForAllMembers<Person>(
   //      [&](const auto& member)
   //      {
   //         using MemberT = meta::get_member_type<decltype(member)>;
   //         MemberT td = member.get(person);
   //         std::cout << typeid(MemberT).name() << std::endl;
   //         std::cout << member.getName() << " : " << td << std::endl;
   //         //std::cout << member << std::endl;
   //      });
}  