#pragma once

class COM {
public:
	COM();
	~COM();

	constexpr explicit operator bool() { return m_ok; }

private:
	bool m_ok = false;
};
