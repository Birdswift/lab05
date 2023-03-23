#include <Account.h>
#include <gtest/gtest.h>
#include <Account.h>
#include <Transaction.h>
#include <gtest/gtest.h>

TEST(Account, Banking){
	Account test(0,0); //constructor
	ASSERT_EQ(test.GetBalance(), 0); 
	// lock test
	ASSERT_THROW(test.ChangeBalance(100), std::runtime_error); // add balance
	test.Lock(); //lock
	ASSERT_NO_THROW(test.ChangeBalance(100)); //can we change?
//change balance, check balance
	ASSERT_EQ(test.GetBalance(), 100);
//if it locked?
	ASSERT_THROW(test.Lock(), std::runtime_error);
//check unlock
	test.Unlock();
	ASSERT_THROW(test.ChangeBalance(100), std::runtime_error);
}

TEST(Transaction, Banking){
	const int base_A = 5000, base_B = 5000, base_fee = 100;
	Account Man1(0,base_A), Man2(1,base_B);
	Transaction tran;
//constructors
	ASSERT_EQ(tran.fee(), 1);
	tran.set_fee(base_fee);
	ASSERT_EQ(tran.fee(), base_fee);
//when transaction doesnt work
	ASSERT_THROW(tran.Make(Man1, Man1, 1000), std::logic_error);
	ASSERT_THROW(tran.Make(Man1, Man2, -50), std::invalid_argument);
	ASSERT_THROW(tran.Make(Man1, Man2, 50), std::logic_error);
	if (tran.fee()*2-1 >= 100)
		ASSERT_EQ(tran.Make(Man1, Man2, tran.fee()*2-1), false);
//lock
	Man1.Lock();
	ASSERT_THROW(tran.Make(Man1, Man2, 1000), std::runtime_error);
	Man1.Unlock();
	ASSERT_EQ(tran.Make(Man1, Man2, 1000), true); //all good
	ASSERT_EQ(Man2.GetBalance(), base_B+1000);	
	ASSERT_EQ(Man1.GetBalance(), base_A-1000-base_fee);
	ASSERT_EQ(tran.Make(Man1, Man2, 3900), false);//not enough money
	ASSERT_EQ(Man2.GetBalance(), base_B+1000);	
	ASSERT_EQ(Man1.GetBalance(), base_A-1000-base_fee);
}

