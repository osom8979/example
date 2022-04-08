// eslint-disable-next-line @typescript-eslint/no-var-requires
const path = require('path');

// eslint-disable-next-line @typescript-eslint/no-var-requires
const ESLintPlugin = require('eslint-webpack-plugin');

const rootDir = path.resolve(__dirname);
const srcDir = path.join(rootDir, 'src');
const distDir = path.join(rootDir, 'dist');
const isProduction = process.env.NODE_ENV === 'production';
const extensions = ['.jsx', '.js', '.tsx', '.ts'];

const config = {
  entry: path.join(srcDir, 'index.ts'),
  output: {
    path: distDir,
    filename: '[name].js',
    libraryTarget: 'umd',
    library: 'recc-api',
  },
  plugins: [new ESLintPlugin({extensions})],
  module: {
    rules: [
      {
        test: /\.(jsx|js|tsx|ts)$/i,
        exclude: /node_modules/,
        use: {
          loader: 'babel-loader',
          options: {
            presets: ['@babel/preset-env', '@babel/preset-typescript'],
          },
        },
      },
      {
        test: /\.(eot|svg|ttf|woff|woff2|png|jpg|gif)$/i,
        type: 'asset',
      },
    ],
  },
  resolve: {
    extensions,
  },
};

module.exports = () => {
  if (isProduction) {
    config.mode = 'production';
  } else {
    config.mode = 'development';
  }
  return config;
};
