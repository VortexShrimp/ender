# ender
A guide to programming in/with the ender framework.

## General
- Use `snake_case` except for template parameters which should be `PascalCase`.
- Opening braces must always be on the same line.
- Comments should be in English with appropriate grammar & punctuation.
```cpp
// Prefer "class" keyword over "typename".
template <class T, class U, class V>
class my_class {
// Public interface should always come before private.
public:
	void public_method();

private:
	int m_private_member;
}

void some_function(int some_parameter) {
	int my_local_variable;
}
```

## Prefixes
```cpp
// Global variables prefixed with "g_".
int g_some_global = 0;

// Static variables prefixed with "s_".
static int s_some_static = 1;

// Booleans should be prefixed with "is_", "should_" or "use_.
bool is_boolean = true;

class my_class {
public:
	// Public members have no prefix.
	int public_var;

private:
	// Private members prefixed with "m_".
	int m_private_var;
}
```

## Conditionals
- Should be as explicit as possible.
- Should always have opening & closing braces.
```cpp
// Correct.
if (is_boolean == true) {
	return;
}

// Incorrect.
if (is_boolean)
	return;
```
