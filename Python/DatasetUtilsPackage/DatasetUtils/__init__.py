from DatasetUtils import GirderConnector

# Note that the loginState parameter is there for easy testing in the future.
# The testing library can just use a login state rather than having to simulate user input to log in.
# login state is a dictionary containing {username, key} where key is the api key to use for getting an access token

## Returns None if failed to get list, otherwise returns list of dataset names: ['ellipsoid', 'left_atrium', ...]
def getDatasetList(loginState = None):
    GirderConnector.printDataPortalWelcome()
    print('Getting dataset list from the ShapeWorks Portal')
    accessToken = GirderConnector.login(loginState)
    return GirderConnector.getDatasetList(accessToken)


## fileList is list of file path strings 
def downloadDataset(datasetName, destinationPath='.', fileList = None, asZip = True, loginState = None):
    GirderConnector.printDataPortalWelcome()
    print('Downloading the', datasetName, 'dataset from the ShapeWorks Portal')
    accessToken = GirderConnector.login(loginState)
    
    # specifying fileList overrides asZip
    if asZip and not fileList:
        GirderConnector.downloadDatasetZip(accessToken, datasetName, destinationPath)
    else:
        if fileList is not None:
            print('Downloading', len(fileList), 'specified files')
        GirderConnector.downloadDataset(accessToken, datasetName, destinationPath, fileList)

    print('Downloaded the', datasetName, 'dataset from the ShapeWorks Portal.')


def uploadNewDataset(datasetName, datasetPath, loginState = None):
    GirderConnector.printDataPortalWelcome()
    print('Uploading the %s dataset from %s to the ShapeWorks Portal' % (datasetName, datasetPath))
    accessToken = GirderConnector.login(loginState)

    GirderConnector.uploadNewDataset(accessToken, datasetName, datasetPath)

    print('Uploaded the', datasetName, 'dataset to the ShapeWorks Portal.')
