
#ifndef STATIC_IF_H
#define STATIC_IF_H

	template<bool condition, class TypeIfTrue, class TypeIfFale>
	struct StaticIf
	{
		typedef TypeIfTrue Result;
	};

	template<class TypeIfTrue, class TypeIfFale>
	struct StaticIf<false, TypeIfTrue, TypeIfFale>
	{
		 typedef TypeIfFale Result;
	};

#endif /* STATIC_IF_H */