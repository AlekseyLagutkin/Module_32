#include <iostream>
#include <gtest/gtest.h>
#include <gmock/gmock.h>

using namespace std;

class dbcon   //Класс для работы с БД
{
public:
	dbcon()
	{}
	~dbcon()
	{}
	virtual bool con_db(string ip, int port)   //Соединение с БД
	{
		return connect;
	}
	virtual int read_db(int a, int b)   //Запрос данных из БД
	{
		return res;
	}
	virtual void write_db(int a, int b, int c)   //Запись данных в БД
	{}
	virtual void discon_db()   //Закрытие соединения с БД
	{}

private:
	bool connect;
	int res;
};

class control   //Тестируемый класс
{
public:
	control(dbcon* _db): db(_db)
	{}
	~control()
	{}
	
	void connect(string ip, int port)
	{
		if (db->con_db(ip, port))
		{
			cout << "Соединение с БД установлено" << endl;
		}
		else
		{
			cout << "Нет соединения с БД" << endl;
		}
	}
	int get_value(int a, int b)
	{
		int c = db->read_db(a, b);
		cout << "value = " << c << endl;
		return c;
	}

	void set_value(int a, int b, int c)
	{
		db->write_db(a, b, c);
	}
	void discon()
	{
		db->discon_db();
	}

private:
	dbcon* db;
};

class Mockdb: public dbcon
{
public:
	MOCK_METHOD(bool, con_db, (string ip, int port), (override));
	MOCK_METHOD(int, read_db, (int a, int b), (override));
	MOCK_METHOD(void, write_db, (int a, int b, int c), (override));
	MOCK_METHOD(void, discon_db, (), (override));
};

TEST(controltest, test1)
{
	Mockdb mdb;
	EXPECT_CALL(mdb, con_db("192.168.0.10", 502)).WillOnce(::testing::Return(true));
	EXPECT_CALL(mdb, read_db(20, 4)).WillOnce(::testing::Return(3));
	EXPECT_CALL(mdb, write_db).Times(::testing::AtLeast(2));
	EXPECT_CALL(mdb, discon_db).Times(1);

	control c1(&mdb);
	c1.connect("192.168.0.10", 502);
	c1.get_value(20, 4);
	c1.set_value(10, 25, 8);
	c1.set_value(12, 100, 4);
	c1.discon();
}

int main()
{
	setlocale(LC_ALL, "");
	::testing::InitGoogleMock();
	return RUN_ALL_TESTS();
}