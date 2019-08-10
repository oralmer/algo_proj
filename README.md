# Side channel cryptographic attacks - Password brute-forcing project

This project is an implementation of a password brute-forcer. It supports brute-forcing a password using a cryptographic side channel vulnerability in WPA3.

The project contains 3 main parts:

1. Python based database admin
2. Python based worker manager
3. C++ based worker

## Database Admin

The DB admin takes the structure of the password, and the collected side-channel data, and uses them to construct the DB. It then displays possible passwords found by the workers to the user.

basic usage (Can also take DB params such as port, password, etc.):

`DB_adm <start index> <end index> <hashing json> <password json> <split length>`

The workers will then work according to the JSONs, each time querying a split of length <split length> from the DB, until all passwords from  <start index> to <end index> have been checked.

For examples of JSON structure, see the CPP worker's tests.

### DB structure:
**work_range**: A single task for one worker.

- start index\end index
- status (free\in_progress\done)
- pass_setting (foreign key)
- last_updated (to allow other workers to pick up ranges that are stuck\failed)

**password_setting**: Contains the hashing and password structure JSONS.

**found_passwords**: Found passwords and a key to the associated password_setting.

**dict_words + dictionaries**: A dictionary's name and words. 

## Worker manager

The worker opens multiple subprocesses. each subprocess then queries the DB for a work range, and calls the CPP worker on it.

basic usage example (Can also take DB params such as port, password, etc.):

`worker <path to exe>` 

## Worker

Contains 2 main sections - password generation and hashing.

Password generation allows generating passwords by parameters and index. Currently supported generation types are:

- **Character**: Takes a list of chars
- **Dictionary**: Takes a dict and loads it into memory for fast performance
- **Big dictionary**: Takes a dict and loads words directly from file, to avoid using too much memory 
- **Cartesian**: Takes n sub generators and returns a concatenation of all n.
- **Or**: Takes n sub generators and returns each one in turn

You can easily add more generators by inheriting from IPassGen, and adding them to the factory.

Currently supported hashing types are:

- **ID hashing**: Takes a string, returns whether a given password is that string (created for testing)
- **EC Hashing**: Takes side channel data collected on a WPA3 router in ECC mode. Then checks if a given password could have created that data.
- **Mod P Hashing**: Takes side channel data collected on a WPA3 router in Mod P encryption mode. Then checks if a given password could have created that data.

You can easily add more hashers by inheriting from IHasher, and adding them to the factory.

