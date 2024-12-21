const webpack = require('@nativescript/webpack');

module.exports = (env) => {
  webpack.init(env);
  
  // Add platform-specific configs
  webpack.chainWebpack((config) => {
    config.resolve.extensions
      .add('.android.ts')
      .add('.ios.ts');
  });

  return webpack.resolveConfig();
};