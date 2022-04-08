const path = require('path');

const rootDir = path.resolve(__dirname);
const srcDir = path.join(rootDir, 'src');
const distDir = path.join(rootDir, 'dist');

module.exports = {
  context: srcDir,
  mode: 'development',
  devtool: 'inline-source-map',
  entry: path.join(srcDir, 'index.ts'),
  module: {
    rules: [
      {
        test: /\.m?js$/,
        exclude: /node_modules/,
        use: {
          loader: 'babel-loader',
          options: {
            presets: [
              '@babel/preset-env',
              '@babel/preset-typescript',
              '@babel/preset-flow',
            ],
          },
        },
      },
    ],
  },
  resolve: {
    extensions: ['.tsx', '.ts', '.js'],
  },
  output: {
    path: distDir,
    filename: '[name].js',
    libraryTarget: 'umd',
    library: 'recc-api',
  },
};
