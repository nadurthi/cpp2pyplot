def readBinaryDataSet(metaFilePath,binaryFilePath):
	dfmeta = pd.read_csv(metaFilePath)
	binaryFile  = open(binaryFilePath,'rb')
	# print binaryFile.seek()

	FileVars={}
	for ind in dfmeta.index: 	
		binaryFile.seek(dfmeta.loc[ind,'startBytePosition'])
		typenum = None
		if dfmeta.loc[ind,'internalVartype'] == 'int':
			typenum = np.int32
		if dfmeta.loc[ind,'internalVartype'] == 'double':
			typenum = np.float64
		if dfmeta.loc[ind,'internalVartype'] == 'float':
			typenum = np.float

		S = np.fromfile(binaryFile, dtype=typenum,count = dfmeta.loc[ind,'count'])
		if dfmeta.loc[ind,'nrows']>0 and dfmeta.loc[ind,'ncols']>0:
			S = S.reshape((dfmeta.loc[ind,'nrows'],dfmeta.loc[ind,'ncols']),order='F')
		FileVars[dfmeta.loc[ind,'variable_name']] = S
		# break

	return FileVars