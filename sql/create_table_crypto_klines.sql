CREATE TABLE IF NOT EXISTS crypto_klines (
    id SERIAL PRIMARY KEY,
    symbol VARCHAR(10) NOT NULL,
    open_time BIGINT NOT NULL,
    open NUMERIC(18,8) NOT NULL,
    high NUMERIC(18,8) NOT NULL,
    low NUMERIC(18,8) NOT NULL,
    close NUMERIC(18,8) NOT NULL,
    volume NUMERIC(24,10) NOT NULL,
    close_time BIGINT NOT NULL,
    quote_asset_volume NUMERIC(24,10) NOT NULL,
    trades_count INTEGER NOT NULL,
    taker_buy_base_volume NUMERIC(24,10) NOT NULL,
    taker_buy_quote_volume NUMERIC(24,10) NOT NULL,
    UNIQUE (symbol, open_time, close_time)
)