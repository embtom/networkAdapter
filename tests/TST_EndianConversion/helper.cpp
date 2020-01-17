template<typename Class>
class SerializerFunc
{
public:
   SerializerFunc(Class& rObj) : m_rObj(rObj) {};
   template<typename Member>
   void operator()(Member& member)
   {
      using entry = meta::get_member_type<Member>;
      entry a = member.get(m_rObj);
      
      entry b = EtNet::host_to_network(a); 
      member.set(m_rObj, b);
      std::cout << std::hex << a << " " << b << std::endl;
      //meta::get_member_type<Member> a = 5;

      // EtNet::host_to_network(a);
      // if (member.canGetConstRef()) {
      //    auto valueName = member.get(m_rObj);
      // } else if (member.hasGetter()) {
      //    auto valueName = member.getCopy(m_rObj); // passing copy as const ref, it's okay
      // }
   }
   
   //  template<typename Member,
   //           typename = std::enable_if_t<!is_optional< meta::get_member_type<Member> >::value>
   //  >
   //  void operator()(Member& member)
   //  {
   //      auto& valueName = m_rValue[member.getName()];
   //      if (member.canGetConstRef()) {
   //          valueName = member.get(m_rObj);
   //      } else if (member.hasGetter()) {
   //          valueName = member.getCopy(m_rObj); // passing copy as const ref, it's okay
   //      }
   //  }
   //  template<typename Member,
   //           typename = std::enable_if_t<is_optional< meta::get_member_type<Member> >::value>,
   //           typename = void
   //  >
   //  void operator()(Member& member)
   //  {
   //      if (member.canGetConstRef()) {
   //          auto& rOptional = member.get(m_rObj);
   //          if(rOptional)
   //          {
   //              auto& valueName = m_rValue[member.getName()];
   //              valueName = *member.get(m_rObj);
   //          }
   //      }
   //  }
private:
   Class&    m_rObj;
};