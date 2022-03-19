int main()
{
	// read lines from console
	vector<string> lines;
	string line;
	while (getline(cin, line))
	{
        line.shrink_to_fit();
		if (!line.empty())
			lines.push_back(line);
	}

	// parse lines to pairs
	size_t size = lines.size();
	pair_t* array = new pair_t[size];
	for (size_t i = 0; i < size; i++)
	{
		array[i] = ParseLine(lines[i]);
	}
    
	// sorting
	CSorter::RadixSort(array, size);

	// output results
	for (size_t i = 0; i < size; i++)
	{
        puts(array[i].GetValue());
	}
}
