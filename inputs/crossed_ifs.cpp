bool crossed_ifs(int i, double j, char* k)
{
	if (i == 0)
		return true;
	if (0. == j)
		return true;
	if (k == nullptr)
		return true;
	return false;
}
