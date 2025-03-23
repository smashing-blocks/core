using namespace QPI;

struct HM252
{
};

struct HM25 : public ContractBase
{
public:
    // Structure to store wallet data entries with an ID and a value
    struct WalletDataEntry {
        id walletId;
        uint64 value;
    };

    // Input/Output structures for SaveWalletData procedure
    struct SaveWalletData_input {
        id walletId;
        uint64 value;
    };
    struct SaveWalletData_output {};

    // Input/Output/Locals structures for GetStoredData function
    struct GetStoredData_input {
        uint64 index;
    };
    struct GetStoredData_output {
        id walletId;
        uint64 value;
    };
    struct GetStoredData_locals {
        WalletDataEntry entry;
    };

protected:
    // Contract state variables
    uint64 totalRewards;        // Accumulates all rewards received
    uint64 numberOfEntries;     // Counter for number of entries in the array
    Array<WalletDataEntry, 1024> walletDataEntries;  // Array to store wallet data


    PUBLIC_PROCEDURE(SaveWalletData)
        // Create new entry with input data
        WalletDataEntry newEntry;
        newEntry.walletId = input.walletId;
        newEntry.value = input.value;

        // Check if there's space and save to array
        if (state.numberOfEntries < 1024)
        {
            state.walletDataEntries.set(state.numberOfEntries, newEntry);
            state.numberOfEntries++;
        }

        // Store any invocation rewards in the contract
        if (qpi.invocationReward() > 0)
        {
            state.totalRewards += qpi.invocationReward();
        }
    _


    PUBLIC_FUNCTION_WITH_LOCALS(GetStoredData)
   
        if (input.index < state.numberOfEntries)
        {
            locals.entry = state.walletDataEntries.get(input.index);
            output.walletId = locals.entry.walletId;
            output.value = locals.entry.value;
        }
    _

    // Register contract functions and procedures
    REGISTER_USER_FUNCTIONS_AND_PROCEDURES
        REGISTER_USER_PROCEDURE(SaveWalletData, 1);
        REGISTER_USER_FUNCTION(GetStoredData, 2);
    _

    // Initialize contract state
    INITIALIZE
        state.totalRewards = 0;
        state.numberOfEntries = 0;
    _
};
