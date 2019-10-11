#include <iostream>
#include <memory>

class Foo {
	std::weak_ptr<Foo> ptr_;

public:
	Foo()
	{
		std::cout << "Foo\n";
	};
	~Foo()
	{
		std::cout << "~Foo\n";
	};

	void setPtr(std::shared_ptr<Foo> p)
	{
		ptr_ = p;
	};
};


void test() {
	std::shared_ptr<Foo> _f1 = std::make_shared<Foo>(Foo());
	std::shared_ptr<Foo> _f2 = std::make_shared<Foo>(Foo());

	std::cout << "use_count: " << _f1.use_count() << "\n";
	std::cout << "use_count: " << _f2.use_count() << "\n";

	(*_f1).setPtr(_f2);
	(*_f2).setPtr(_f1);

	std::cout << "use_count: " << _f1.use_count() << "\n";
	std::cout << "use_count: " << _f2.use_count() << "\n";
}

int main(int argc, char ** argv) {

	test();

	return 0;
}
