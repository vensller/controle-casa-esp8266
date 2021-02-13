import React from 'react';
import {createAppContainer, createSwitchNavigator} from 'react-navigation';
import {createBottomTabNavigator} from 'react-navigation-tabs';

import Dashboard from '~/pages/Dashboard';
import Settings from '~/pages/Config';

export default () =>
  createAppContainer(
    createSwitchNavigator(
      {        
        App: createBottomTabNavigator(
          {
            Dashboard,
            Settings
          },
          {
            resetOnBlur: true,
            tabBarOptions: {
              keyboardHidesTabBar: true,
              activeTintColor: '#FFF',
              inactiveTintColor: 'rgba(255, 255, 255, 0.6)',
              style: {
                backgroundColor: '#8d41a8',
              },
            },
          },
        ),
      },
      {
        initialRouteName: 'App',
      },
    ),
  );
