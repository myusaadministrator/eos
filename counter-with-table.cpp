#include <eosiolib/eosio.hpp>

//@abi table
struct counter{
  uint64_t actor;
  uint64_t value;
  uint64_t primary_key() const { return actor; }
};
typedef eosio::multi_index<N(counter),counter> counter_table;

class counter_contract:eosio::contract{
  public:
    counter_contract(account_name self):eosio::contract(self){}
    void increase(account_name actor){
      eosio::require_auth(actor);
      value = get_value(actor);
      eosio::print(eosio::name{actor} , " INCREASE => ",value++);
      update_value(actor,value);
    }
    void decrease(account_name actor){
      eosio::print(eosio::name{actor} , " DECREASE => ",value--);
    }
  private:
    uint64_t value;
  
  	uint64_t get_value(account_name actor){
      counter_table c(_self,_self);
      auto iter = c.find(actor);

      if(iter != c.end()) return iter->value;
      
      c.emplace(actor,[&](auto& record){
        record.actor = actor;
        record.value = 0;
      });
      
      return 0;      
    }
    
    void update_value(account_name actor,uint64_t value){
      counter_table c(_self,_self);
      auto iter = c.find(actor);
      c.modify(iter,actor,[&](auto& record){
        record.value = value;
      });
    }
};

EOSIO_ABI(counter_contract,(increase)(decrease))



