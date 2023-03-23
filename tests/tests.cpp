#include <Account.h>
#include <Transaction.h>
#include <gtest/gtest.h>

//THIS FILE TESTS BOTH ACCOUNT.CPP AND TRANSACTION.CPP FILES
TEST(Banking, Account) {
Account acc(0, 0); //Account creation

ASSERT_EQ(acc.GetBalance(), 0); //Checking the constructor
ASSERT_THROW(acc.ChangeBalance(1234), std::runtime_error); //Checking if unlocked
acc.Lock(); //Let's lock it...
ASSERT_NO_THROW(acc.ChangeBalance(1234)); //Checking if locked
ASSERT_THROW(acc.Lock(), std::runtime_error); //Sure that we cannot lock this dude twice
ASSERT_EQ(acc.GetBalance(), 1234); //Checking the balance after we have change it
acc.ChangeBalance(-1235); //Let's do some %bad_words%
ASSERT_EQ(acc.GetBalance(), -1); //How could it be...?
acc.Unlock(); //Dobby is free
ASSERT_THROW(acc.ChangeBalance(1234), std::runtime_error); //Checking if it's right
}

TEST(Banking, Transaction) {
Account acc1(1, 1500);
Account acc2(2, 1500);
Transaction transaction;

ASSERT_EQ(transaction.fee(), 1); //Checking the constructor
transaction.set_fee(20);  //Let the fee be 20
ASSERT_EQ(transaction.fee(), 20); //Times getting hard...

ASSERT_THROW(transaction.Make(acc1, acc2, -345), std::invalid_argument); //Are u trying to steal some money?
ASSERT_THROW(transaction.Make(acc2, acc2, 1000), std::logic_error); //You better try it again
ASSERT_THROW(transaction.Make (acc1, acc2, 40), std::logic_error); //It's too small, isn't it?
transaction.set_fee(100); //The Russians have crashed the USA bank system
ASSERT_EQ(transaction.Make(acc1, acc2, 150), false); //Try to pick a sum bigger
ASSERT_EQ(transaction.Make(acc1, acc2, 2000), false); //Wow wow, it's too big, try smaller

ASSERT_EQ(transaction.Make(acc1, acc2, 1000), true); //Finally, a transaction!
ASSERT_EQ(acc2.GetBalance(), 2500);	//Too rich
ASSERT_EQ(acc1.GetBalance(), 400); //LMAO too poor
}
