// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2012 The Bitcoin developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "assert.h"

#include "chainparams.h"
#include "main.h"
#include "util.h"

#include <boost/assign/list_of.hpp>

using namespace boost::assign;

struct SeedSpec6 {
    uint8_t addr[16];
    uint16_t port;
};

#include "chainparamsseeds.h"

//
// Main network
//

// Convert the pnSeeds6 array into usable address objects.
static void convertSeed6(std::vector<CAddress> &vSeedsOut, const SeedSpec6 *data, unsigned int count)
{
    // It'll only connect to one or two seed nodes because once it connects,
    // it'll get a pile of addresses with newer timestamps.
    // Seed nodes are given a random 'last seen time' of between one and two
    // weeks ago.
    const int64_t nOneWeek = 7*24*60*60;
    for (unsigned int i = 0; i < count; i++)
    {
        struct in6_addr ip;
        memcpy(&ip, data[i].addr, sizeof(ip));
        CAddress addr(CService(ip, data[i].port));
        addr.nTime = GetTime() - GetRand(nOneWeek) - nOneWeek;
        vSeedsOut.push_back(addr);
    }
}

class CMainParams : public CChainParams {
public:
    CMainParams() {
        // The message start string is designed to be unlikely to occur in normal data.
        // The characters are rarely used upper ASCII, not valid as UTF-8, and produce
        // a large 4-byte int at any alignment.
        pchMessageStart[0] = 0x44;
        pchMessageStart[1] = 0x45;
        pchMessageStart[2] = 0x53;
        pchMessageStart[3] = 0x54;
        vAlertPubKey = ParseHex("04acdb4977a559440c1fa273cc79ef738ece2bfdc984ebcbd1defad4656b30d13d97a5f64448eb5ff4fd56420b340bf5d63ae57e929b666d5717f853a629103f61");
        vchSyncCheckpointPubKey = ParseHex("0448addbefe39432f89547603c7016cac247ad9fdffa34459d5534cf0d64f3afec95a4f20c3d80941cdc771fa2ec4a652488dcfe8cc6abf5f3f2ab05a79f8bba6e");
        nDefaultPort = 17100;
        nRPCPort = 17200;
        bnProofOfWorkLimit = CBigNum(~uint256(0) >> 18);

        // Build the genesis block. Note that the output of the genesis coinbase cannot
        // be spent as it did not originally exist in the database.
        const char* pszTimestamp = "NYTimes Harper Lee, Author of the Beloved ‘To Kill a Mockingbird,’ Dies at 89"; 
        std::vector<CTxIn> vin;
        vin.resize(1);
        vin[0].scriptSig = CScript() << 0 << CBigNum(42) << vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
        std::vector<CTxOut> vout;
        vout.resize(1);
        vout[0].SetEmpty();
        CTransaction txNew(1, 1455913800, vin, vout, 0);
        genesis.vtx.push_back(txNew);
        genesis.hashPrevBlock = 0;
        genesis.hashMerkleRoot = genesis.BuildMerkleTree();
        genesis.nVersion = 1;
        genesis.nTime    = 1455913800;
        genesis.nBits    = bnProofOfWorkLimit.GetCompact();
        genesis.nNonce   = 120558;

        hashGenesisBlock = uint256("0x00002d08f49d3d5b084eba92059d00386c038af409a74814d5af01794499d2ce");
        if (true && (genesis.GetHash() != hashGenesisBlock))
        {
            uint256 hashTarget = CBigNum().SetCompact(genesis.nBits).getuint256();
            while (genesis.GetHash() > hashTarget)
            {
                ++genesis.nNonce;
                if (genesis.nNonce == 0)
                {
                    printf("NONCE WRAPPED, incrementing time");
                    ++genesis.nTime;
                }
            }
            cout << "genesis: \n" << genesis.ToString() << endl;
            cout << "genesis.GetHash(): " << genesis.GetHash().ToString() << endl;
            cout << "genesis.hashMerkleRoot: " << genesis.hashMerkleRoot.ToString() << endl;
            cout << "genesis.nTime: " << genesis.nTime << endl;
            cout << "genesis.nNonce: " << genesis.nNonce << endl;
        }


        assert(hashGenesisBlock == uint256("0x00002d08f49d3d5b084eba92059d00386c038af409a74814d5af01794499d2ce"));
        assert(genesis.hashMerkleRoot == uint256("0x2aea7c3766b9701a92dd0223a72d5fc97488b2fcb3229a525c30dc4eb70d658e"));

        base58Prefixes[PUBKEY_ADDRESS] = list_of(30)(101)(250);
        base58Prefixes[SCRIPT_ADDRESS] = list_of(85)(85)(85);
        base58Prefixes[SECRET_KEY] =     list_of(153)(153)(153);
        base58Prefixes[EXT_PUBLIC_KEY] = list_of(0x04)(0x88)(0xB2)(0x1E);
        base58Prefixes[EXT_SECRET_KEY] = list_of(0x04)(0x88)(0xAD)(0xE4);

        convertSeed6(vFixedSeeds, pnSeed6_main, ARRAYLEN(pnSeed6_main));

        bnProofOfStakeLimit = CBigNum(~uint256(0)>>5);
        
        nBasicPoWReward = 16 * COIN; 
        nPoWHalving = 6750; 
        nLaunchTime = genesis.nTime;
        nMinDelay = 2; //seconds

        nCoinbaseMaturity = 20;
        nStakeMinConfirmations = 20;

        nStakeMinAge = 60 * 60; 
        nModifierInterval = 10 * 60; // time to elapse before new modifier is computed

        nStakeCoinYearReward = 10 * CENT;
        nPoSHalving = 1350 * 365; //

        nLastPOWBlock = 40500;
        nFirstPoSBlock = 6750;
        nPoSGranularity = 15;
        nTargetSpacing =  (nPoSGranularity + 1) * 4;

        nMaxMoney = nLastPOWBlock * nBasicPoWReward;

    }

    virtual const CBlock& GenesisBlock() const { return genesis; }
    virtual Network NetworkID() const { return CChainParams::MAIN; }

    virtual const vector<CAddress>& FixedSeeds() const {
        return vFixedSeeds;
    }
protected:
    CBlock genesis;
    vector<CAddress> vFixedSeeds;
};
static CMainParams mainParams;


//
// Testnet
//

class CTestNetParams : public CMainParams {
public:
    CTestNetParams() {
        // The message start string is designed to be unlikely to occur in normal data.
        // The characters are rarely used upper ASCII, not valid as UTF-8, and produce
        // a large 4-byte int at any alignment.
        pchMessageStart[0] = 0x64;
        pchMessageStart[1] = 0x65;
        pchMessageStart[2] = 0x73;
        pchMessageStart[3] = 0x74;
        bnProofOfWorkLimit = CBigNum(~uint256(0) >> 16);
        vAlertPubKey = ParseHex("04acdb4977a559440c1fa273cc79ef738ece2bfdc984ebcbd1defad4656b30d13d97a5f64448eb5ff4fd56420b340bf5d63ae57e929b666d5717f853a629103f61");
        nDefaultPort = 28100;
        nRPCPort = 28200;
        strDataDir = "testnet";

        // Modify the testnet genesis block so the timestamp is valid for a later start.
        genesis.nBits  = bnProofOfWorkLimit.GetCompact();
        genesis.nNonce = 513682;
        hashGenesisBlock = uint256("0x00002795db710e9cc18988c9f505304b0564b78518162deba23232ec6dfff150");
        if (true && (genesis.GetHash() != hashGenesisBlock))
        {
            cout << "genesis: TEST___!!!"<<endl;  
            uint256 hashTarget = CBigNum().SetCompact(genesis.nBits).getuint256();
            while (genesis.GetHash() > hashTarget)
            {
                ++genesis.nNonce;
                if (genesis.nNonce == 0)
                {
                    printf("NONCE WRAPPED, incrementing time");
                    ++genesis.nTime;
                }
            }
            cout << "genesis: \n" << genesis.ToString() << endl;
            cout << "genesis.GetHash(): " << genesis.GetHash().ToString() << endl;
            cout << "genesis.hashMerkleRoot: " << genesis.hashMerkleRoot.ToString() << endl;
            cout << "genesis.nTime: " << genesis.nTime << endl;
            cout << "genesis.nNonce: " << genesis.nNonce << endl;
        }



        assert(hashGenesisBlock == uint256("0x00002795db710e9cc18988c9f505304b0564b78518162deba23232ec6dfff150"));

        vFixedSeeds.clear();
        vSeeds.clear();

        base58Prefixes[PUBKEY_ADDRESS] = list_of(111);
        base58Prefixes[SCRIPT_ADDRESS] = list_of(196);
        base58Prefixes[SECRET_KEY]     = list_of(239);
        base58Prefixes[EXT_PUBLIC_KEY] = list_of(0x04)(0x35)(0x87)(0xCF);
        base58Prefixes[EXT_SECRET_KEY] = list_of(0x04)(0x35)(0x83)(0x94);

        convertSeed6(vFixedSeeds, pnSeed6_test, ARRAYLEN(pnSeed6_test));

        bnProofOfStakeLimit = CBigNum(~uint256(0) >> 20);

        nCoinbaseMaturity = 10;
        nStakeMinConfirmations = 10;

        nStakeMinAge = 8 * 60; // 8 mins
        nModifierInterval = 10 * 60; // time to elapse before new modifier is computed

        nStakeCoinYearReward = 5 * CENT; // 5% per year

        nLastPOWBlock = 1350*5*6;

        nMaxMoney = 3141592654 * COIN;

    }
    virtual Network NetworkID() const { return CChainParams::TESTNET; }
};
static CTestNetParams testNetParams;
static CChainParams *pCurrentParams = &mainParams;

const CChainParams &Params() {
    return *pCurrentParams;
}
void SelectParams(CChainParams::Network network) {
    switch (network) {
        case CChainParams::MAIN:
            pCurrentParams = &mainParams;
            break;
        case CChainParams::TESTNET:
            pCurrentParams = &testNetParams;
            break;
        default:
            assert(false && "Unimplemented network");
            return;
    }
}

bool SelectParamsFromCommandLine() {
    bool fTestNet = GetBoolArg("-testnet", false);

    if (fTestNet ) {
        return false;
    }

    if (fTestNet) {
        SelectParams(CChainParams::TESTNET);
    } else {
        SelectParams(CChainParams::MAIN);
    }
    return true;
}


