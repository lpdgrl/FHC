INSERT INTO crypto_klines 
(symbol, open_time, open, high, low, close, volume, close_time, quote_asset_volume, trades_count, taker_buy_base_volume, taker_buy_quote_volume) 
VALUES ($1, $2, $3, $4, $5, $6, $7, $8, $9, $10, $11, $12)