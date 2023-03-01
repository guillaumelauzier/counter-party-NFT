import counterpartylib

# Connect to the Counterparty API
api = counterpartylib.server.RpcRawProxy()

# Create a new asset
asset_name = "MyNFT"
asset_description = "My first NFT on Counterparty"
asset_data = {
    "artist": "John Smith",
    "year": 2023,
    "medium": "Oil on canvas"
}
asset = api.create_issuance(
    source="myaddress",
    asset=asset_name,
    quantity=1,
    description=asset_description,
    divisible=False,
    callable_=False,
    transfer_destination="myaddress",
    data=asset_data
)

# Issue the new asset
tx_hash = api.sendrawtransaction(asset)
print("Transaction hash:", tx_hash)
