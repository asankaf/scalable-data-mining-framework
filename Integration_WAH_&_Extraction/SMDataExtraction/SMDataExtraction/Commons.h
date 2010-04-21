#pragma once

class Commons
{
public:
	Commons(void);
	~Commons(void);
	
	template <typename iter_type>
	static void DeleteVector(iter_type first, iter_type last)
	{
		for (iter_type cur = first; cur != last; ++cur)
		{
			if((*cur) != NULL)
			{
				delete *cur;
				*cur = NULL;
			}
		}
	}

	template <typename iter_type>
	static void InitVector(iter_type first, iter_type last)
	{
		for (iter_type cur = first; cur != last; ++cur)
		{

			*cur = NULL;
		}
	}
	
};
