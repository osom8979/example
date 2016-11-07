
#include <stdio.h>
#include <hdf5.h>

int main()
{
    // create a dataset that is a 4 x 6 array.
    char const * const DB_FILE = "dset.h5";

    // identifiers.
    hid_t    file_id, dataset_id, dataspace_id;
    hsize_t  dims[2];
    herr_t   status;

    // Create a new file using default properties.
    file_id = H5Fcreate(DB_FILE, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);

    // Create the data space for the dataset.
    dims[0] = 4; 
    dims[1] = 6; 
    dataspace_id = H5Screate_simple(2, dims, NULL);

    // Create the dataset.
    dataset_id = H5Dcreate2(file_id, "/dset", H5T_STD_I32BE, dataspace_id, 
                          H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);

    // End access to the dataset and release resources used by it.
    status = H5Dclose(dataset_id);
    if (status < 0) {
        printf("h5d close dataset error(%d)\n", status);
        return 1;
    }

    // Terminate access to the data space.
    status = H5Sclose(dataspace_id);
    if (status < 0) {
        printf("h5d close dataspace error(%d)\n", status);
        return 1;
    }

    // Close the file.
    status = H5Fclose(file_id);
    if (status < 0) {
        printf("h5d close file error(%d)\n", status);
        return 1;
    }

    return 0;
}

