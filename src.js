const axios = require('axios');

// Set up the Counterparty API endpoint
const CP_API = 'http://localhost:14000/api/';

// Create a new asset
const asset_name = 'MyNFT';
const asset_description = 'My first NFT on Counterparty';
const asset_data = {
  artist: 'John Smith',
  year: 2023,
  medium: 'Oil on canvas'
};
const issuance_data = {
  source: 'myaddress',
  asset: asset_name,
  quantity: 1,
  description: asset_description,
  divisible: false,
  callable_: false,
  transfer_destination: 'myaddress',
  data: asset_data
};

axios.post(CP_API + 'create_issuance', issuance_data)
  .then(response => {
    console.log('Asset created:', response.data);

    // Issue the new asset
    const tx_data = {
      tx_hex: response.data,
      allow_unconfirmed_inputs: true
    };

    axios.post(CP_API + 'send_tx', tx_data)
      .then(response => {
        console.log('Asset issued:', response.data);
      })
      .catch(error => {
        console.error('Error issuing asset:', error.response.data);
      });
  })
  .catch(error => {
    console.error('Error creating asset:', error.response.data);
  });
